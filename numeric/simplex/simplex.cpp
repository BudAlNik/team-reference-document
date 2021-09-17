struct simplex_t {
    vector<vector<double>> mat;
    int EQ, VARS, p_row;

    vector<int> column;
    
    void row_subtract(int what, int from, double x) {
        for (int i = 0; i <= VARS; ++i)
            mat[from][i] -= mat[what][i] * x;
    }

    void row_scale(int what, double x) {
        for (int i = 0; i <= VARS; ++i)
            mat[what][i] *= x;
    }

    void pivot(int var, int eq) {
        row_scale(eq, 1. / mat[eq][var]);
        
        for (int p = 0; p <= EQ; ++p)
            if (p != eq)
                row_subtract(eq, p, mat[p][var]);

        column[eq] = var;
    }
    
    void iterate() {
        while (true) {
            int j = 0;
            for (; j != VARS and mat[EQ][j] < eps; ++j) {}

            if (j == VARS)
                break;

            double lim = 1e100;
            int arg_min = -1;
        
            for (int p = 0; p != EQ; ++p) {
                if (mat[p][j] < eps)
                    continue;

                double newlim = mat[p][VARS] / mat[p][j];
                if (newlim < lim)
                    lim = newlim, arg_min = p;
            }

            if (arg_min == -1)
                throw "unbounded";

            pivot(j, arg_min);
        }
    }
    
    simplex_t(const vector<vector<double>>& mat_): mat(mat_) {
        for (int i = 0; i < SZ(mat); ++i) // fictuous variable
            mat[i].insert(mat[i].begin() + SZ(mat[i]) - 1, double(0));
        
        EQ = SZ(mat), VARS = SZ(mat[0]) - 1;
        column.resize(EQ, -1);
        p_row = 0;
    
        for (int i = 0; i < VARS; ++i) {
            int p;
            for (p = p_row; p < EQ and abs(mat[p][i]) < eps; ++p) {}

            if (p == EQ)
                continue;

            swap(mat[p], mat[p_row]);
            column[p_row] = i;
            row_scale(p_row, 1. / mat[p_row][i]);

            for (p = 0; p != EQ; ++p)
                if (p != p_row)
                    row_subtract(p_row, p, mat[p][i]);
        
            p_row += 1;
        }

        for (int p = p_row; p < EQ; ++p)
            if (abs(mat[p][VARS]) > eps)
                throw "unsolvable (bad equalities)";
    
        if (p_row) {
            int minr = 0;
            for (int i = 0; i < p_row; ++i)
                if (mat[i][VARS] < mat[minr][VARS])
                    minr = i;
    
            if (mat[minr][VARS] < -eps) {
                mat.push_back(vector<double>(VARS + 1));
                
                mat[EQ][VARS - 1] = -1;
                for (int i = 0; i != p_row; ++i)
                    mat[i][VARS - 1] = -1;
            
                pivot(VARS - 1, minr);
                iterate();

                if (abs(mat[EQ][VARS]) > eps)
                    throw "unsolvable";

                for (int c = 0; c != EQ; ++c)
                    if (column[c] == VARS - 1) {
                        int p = 0;
                        while (p != VARS - 1 and abs(mat[c][p]) < eps)
                            ++p;
                
                        assert(p != VARS - 1);
                        pivot(p, c);
                        break;
                    }

                for (int p = 0; p != EQ; ++p)
                    mat[p][VARS - 1] = 0;
                
                mat.pop_back();
            }
        }
    }
    
    double solve(vector<double> coeff, vector<double>& pans) {
        auto mat_orig = mat;
        auto col_orig = column;
        
        coeff.resize(VARS + 1);
        mat.push_back(coeff);

        for (int i = 0; i != p_row; ++i)
            row_subtract(i, EQ, mat[EQ][column[i]]);

        iterate();
        
        auto ans = -mat[EQ][VARS];
        if (not pans.empty()) {
            for (int i = 0; i < EQ; ++i) {
                assert(column[i] < VARS);
                pans[column[i]] = mat[i][VARS];
            }
        }
        
        mat = std::move(mat_orig);
        column = std::move(col_orig);
        return ans;
    }

    double solve_min(vector<double> coeff, vector<double>& pans) {
        for (double& elem: coeff)
            elem = -elem;

        return -solve(coeff, pans);
    }
};
