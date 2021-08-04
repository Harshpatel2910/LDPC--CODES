#include <bits/stdc++.h>

#include <fstream>
using namespace std;



int main()
{

    int n = 5000, u = 3000, itr = 0;

    //Update code
    int Update_code[n];

    //H matrix
    int **H = new int *[u];

    for (int i = 0; i < u; i++)
    {
        H[i] = new int[n];
    }
    ifstream myFile;
    myFile.open("H_2.txt");
    int row_in = 0, col_in = 0;

    char value;

    while (!myFile.eof())
    {
        myFile >> value;

        if (value != ',')
        {
            if (col_in == n)
            {
                col_in = 0;
                row_in++;
            }
            H[row_in][col_in] = value - 48;
            col_in++;

            if (row_in == u - 1 && col_in == n)
            {
                break;
            }
        }
    }
    myFile.close();
    //Now we create two 2D array dynamically

    double **v_to_c = new double *[u];
    double **c_to_v = new double *[u];
    double *convergence = new double[100];
    for (int i = 0; i < u; i++)
    {
        v_to_c[i] = new double[n];
        c_to_v[i] = new double[n];

        for (int j = 0; j < n; j++)
        {
            v_to_c[i][j] = 0;
            c_to_v[i][j] = 0;
        }
    }

    // no of 1 in column is wc and no of 1 in row wr

    int wr = 0, wc = 0;
    for (int i = 0; i < u; i++)
    {
        if (H[i][0] == 1)
        {
            wr++;
        }
    }

    for (int i = 0; i < n; i++)
    {
        if (H[0][i] == 1)
        {
            wc++;
        }
    }
    // cout << wr<<" "<<wc<<endl;
    // monte carlo simulation
    int z = 0;
    double ans_graph[10];
    double w = 0.0;
     srand(time(0));

    for (int s = 0; s < 10; s++)
    {
        w += 0.1;
        int prob_succ = 0;
        for (int q = 1; q <= 10; q++)
        {
           

            double code_word[n];

            double probability = w;
            int no_of[n];
            for (int i = 0; i < n; i++)
            {

                 double t = (double)rand() / (double)RAND_MAX;
                int test = (t < probability);

                if (test == 1)

                {
                    code_word[i] = 0.5;
                    no_of[i] = -1;
                }
                else
                {
                    code_word[i] = 0.0;
                    no_of[i] = 0;
                }
            }

            for (int i = 0; i < u; i++)
            {

                for (int j = 0; j < n; j++)
                {
                    v_to_c[i][j] = 0.0;
                    c_to_v[i][j] = 0.0;
                }
            }

            // first iteration vn to cn
            for (int a = 0; a < n; a++)
            {

                for (int j = 0; j < u; j++)
                {

                    if (H[j][a] == 1)
                    {
                        v_to_c[j][a] = code_word[a];
                    }
                }
            }
            // first iteration cn to vn

            for (int i = 0; i < u; i++)
            {

                for (int j = 0; j < n; j++)
                {
                    double parity[3];
                    int idx = 0, count = 0;
                    if (H[i][j] == 1)
                    {

                        for (int k = 0; k < n; k++)
                        {
                            if (H[i][k] == 1 && k != j) //flag
                            {

                                parity[idx] = v_to_c[i][k];
                                if (parity[idx] == 0.5)
                                {
                                    count = 1;
                                    break;
                                }
                                idx++;
                            }
                        }
                    }
                    if (count == 1)
                        c_to_v[i][j] = 0.5;
                    else
                        c_to_v[i][j] = 0.0;
                }
            }
            // UPdate code_word

            // double llr[n];
            int count_e_1 = 0;
            for (int a = 0; a < n; a++)
            {
                double te = code_word[a];
                double code = 1.0;
                for (int k = 0; k < u; k++)
                {

                    if (H[k][a] == 1)
                    {
                        code *= c_to_v[k][a];
                    }
                }
                te = te * code;
                if (te == 0.0)
                    Update_code[a] = 0;
                else
                    Update_code[a] = -1;
            }
            for (int i = 0; i < n; i++)
            {
                if (Update_code[i] == -1)
                    count_e_1++;
            }

            // t>1 iteration vn_to_cn

            for (int t = 2; t < 30; t++)
            {
                for (int i = 0; i < n; i++)
                {

                    for (int j = 0; j < u; j++)
                    {
                        double te = code_word[i];
                        if (H[j][i] == 1)
                        {

                            double code = 1.0;
                            for (int k = 0; k < u; k++)
                            {

                                if (H[k][i] == 1 && k != j)
                                {
                                    code *= c_to_v[k][i];
                                }
                            }
                            te = te * code;
                            if (te == 0.0)
                                v_to_c[j][i] = 0;
                            else
                                v_to_c[j][i] = 0.5;
                        }
                    }
                }

                // t > 1 cn_to_vn

                for (int i = 0; i < u; i++)
                {

                    for (int j = 0; j < n; j++)
                    {

                        double parity[3];
                        int idx = 0, count = 0;
                        if (H[i][j] == 1)
                        {

                            for (int k = 0; k < n; k++)
                            {
                                if (H[i][k] == 1 && k != j) //flag
                                {

                                    parity[idx] = v_to_c[i][k];
                                    if (parity[idx] == 0.5)
                                    {
                                        count = 1;
                                        break;
                                    }
                                    idx++;
                                }
                            }
                        }
                        if (count == 1)
                            c_to_v[i][j] = 0.5;
                        else
                            c_to_v[i][j] = 0.0;
                    }
                }

                for (int a = 0; a < n; a++)
                {
                    double te = code_word[a];
                    double code = 1.0;
                    for (int k = 0; k < u; k++)
                    {

                        if (H[k][a] == 1)
                        {
                            code *= c_to_v[k][a];
                        }
                    }
                    te = te * code;
                    if (te == 0.0)
                        Update_code[a] = 0;
                    else
                        Update_code[a] = -1;
                }
                int count_no_erasure = 0;
                for (int i = 0; i < n; i++)
                {

                    if (Update_code[i] == -1)
                    {
                        count_no_erasure++;
                    }
                }

                if (count_no_erasure == 0)
                {

                    itr = t;
                    break;
                }
            }
            int y = 0;

            for (int i = 0; i < n; i++)
            {
                if (Update_code[i] == -1)
                {
                    y++;
                }
            }

            if (y == 0)
            {
                prob_succ++;
            }
        }

        ans_graph[z] = (double)prob_succ / (double)10;
        z++;
    }

    for (int i = 0; i < 10; i++)
    {
        printf("%lf ", ans_graph[i] * 100.00);
    }

    return 0;
}
