#include <bits/stdc++.h>

#include <fstream>
using namespace std;

double likelihood(double arr[3])
{
    double lamda = (arr[0] / (arr[0] + 1.00)) * (1.00 - ((arr[1] / (arr[1] + 1.00)))) * (1.00 - ((arr[2] / (arr[2] + 1.00)))) + (arr[1] / (arr[1] + 1.00)) * (1.00 - ((arr[0] / (arr[0] + 1.00)))) * (1.00 - ((arr[2] / (arr[2] + 1.00)))) + (arr[2] / (arr[2] + 1.00)) * (1.00 - ((arr[0] / (arr[0] + 1.00)))) * (1.00 - ((arr[1] / (arr[1] + 1.00))));
    lamda += (arr[0] / (arr[0] + 1.00)) * (arr[1] / (arr[1] + 1.00)) * (arr[2] / (arr[2] + 1.00));

    if (lamda != 1.00)
        return lamda / (1.00 - lamda);
    else
        return 1;
}

int main()
{

    int n = 5056, u = 3792, itr = 0, tp;

    //Update code
    int Update_code[n];

    //H matrix
    int **H = new int *[u];

    for (int i = 0; i < u; i++)
    {
        H[i] = new int[n];
    }
    ifstream myFile;
    myFile.open("H_1.txt");
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

    for (int i = 0; i < u; i++)
    {
        v_to_c[i] = new double[n];
        c_to_v[i] = new double[n];

        for (int j = 0; j < n; j++)
        {
            v_to_c[i][j] = 0.0;
            c_to_v[i][j] = 0.0;
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
    double ans_graph[20];
    double w = 0.0;
    srand(time(0));
    for (int s = 0; s < 20; s++)
    {
        w += 0.05;
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
                    code_word[i] = (double)(1.00 - probability) / probability;
                    no_of[i] = 1;
                }
                else
                {
                    code_word[i] = (double)probability / (1.00 - probability);
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
                    int idx = 0;
                    if (H[i][j] == 1)
                    {

                        for (int k = 0; k < n; k++)
                        {
                            if (H[i][k] == 1 && k != j) //flag
                            {

                                parity[idx] = v_to_c[i][k];
                                idx++;
                            }
                        }
                    }

                    c_to_v[i][j] = likelihood(parity);
                }
            }
            // UPdate code_word

            // double llr[n];
            for (int a = 0; a < n; a++)
            {
                double lamda = 1.0;

                if (no_of[a] == 1)
                {
                    lamda *= (1.00 - probability) / probability;
                }
                else
                {
                    lamda *= (double)probability / (1.00 - probability);
                }
                for (int k = 0; k < u; k++)
                {

                    if (H[k][a] == 1)
                    {
                        lamda *= c_to_v[k][a];
                    }
                }

                if (lamda >= 1.00)
                {
                    Update_code[a] = 1;
                }
                else
                {
                    Update_code[a] = 0;
                }
            }
            // t>1 iteration vn_to_cn

            for (int t = 1; t < 35; t++)
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
                            double Lamda = 1.00;
                            if (no_of[i] == 1)
                            {
                                Lamda *= (1.00 - probability) / probability;
                            }
                            else
                            {
                                Lamda *= probability / (1.00 - probability);
                            }
                            Lamda *= te;
                            v_to_c[j][i] = te;
                        }
                    }
                }

                // t > 1 cn_to_vn

                for (int i = 0; i < u; i++)
                {

                    for (int j = 0; j < n; j++)
                    {

                        double parity[3];
                        int idx = 0;
                        if (H[i][j] == 1)
                        {

                            for (int k = 0; k < n; k++)
                            {
                                if (H[i][k] == 1 && k != j) //flag
                                {

                                    parity[idx] = v_to_c[i][k];
                                    idx++;
                                }
                            }
                        }

                        c_to_v[i][j] = likelihood(parity);
                    }
                }

                for (int a = 0; a < n; a++)
                {
                    double lamda = 1.0;

                    if (no_of[a] == 1)
                    {
                        lamda *= (1.00 - probability) / probability;
                    }
                    else
                    {
                        lamda *= (double)probability / (1.00 - probability);
                    }
                    for (int k = 0; k < u; k++)
                    {

                        if (H[k][a] == 1)
                        {
                            lamda *= c_to_v[k][a];
                        }
                    }

                    if (lamda >= 1.00)
                    {
                        Update_code[a] = 1;
                    }
                    else
                    {
                        Update_code[a] = 0;
                    }
                }
                int count_no_1 = 0;
                for (int i = 0; i < n; i++)
                {

                    if (Update_code[i] == 1)
                    {
                        count_no_1++;
                    }
                }

                if (count_no_1 == 0)
                {

                    itr = t;
                    break;
                }
                tp = t;
            }
            int y = 0;

            for (int i = 0; i < n; i++)
            {
                if (Update_code[i] == 1)
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

    for (int i = 0; i < 20; i++)
    {
        printf("%lf ", ans_graph[i] * 100.00);
    }

    return 0;
}
