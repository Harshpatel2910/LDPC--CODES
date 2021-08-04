#include <bits/stdc++.h>

#include <fstream>
using namespace std;

int main()
{

    int n = 12, u = 9, itr = 0;

    int Update_code[n];

    // H matrix

    int H[u][n] = {
        {1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0},
        {1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0},
        {0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0},
        {0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1},
        {0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1},
        {0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0},
        {1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0},
        {0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0},
        {0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1},
    };

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

    //now we create one  2D array

    int **v_to_c = new int *[u];
    int **c_to_v = new int *[u];

    for (int i = 0; i < u; i++)
    {
        v_to_c[i] = new int[n];
        c_to_v[i] = new int[n];

        for (int j = 0; j < n; j++)
        {
            v_to_c[i][j] = 0;
            c_to_v[i][j] = 0;
        }
    }

    // now we perform decoding
    //monte carlo simulation

    int z = 0;

    double ans_graph[25];
    double w = 0.0;
    srand(time(0));
    for (int s = 0; s < 25; s++)
    {
        w += 0.04;
        int prob_succ = 0;

        for (int q = 1; q <= 1000; q++)
        {

            int code_word[n];
            double probability = w;

            for (int i = 0; i < n; i++)
            {

                double t = (double)rand() / (double)RAND_MAX;

                if (t < probability)
                    code_word[i] = -1;

                else
                    code_word[i] = 0;
            }
            for (int i = 0; i < u; i++)
            {

                for (int j = 0; j < n; j++)
                {
                    v_to_c[i][j] = 0;
                    c_to_v[i][j] = 0;
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

                    if (H[i][j] == 1)
                    {
                        int c = 0;
                        for (int k = 0; k < n; k++)
                        {
                            if (H[i][k] == 1 && k != j) //flag
                            {

                                c += v_to_c[i][k];
                            }
                        }
                        // assign c_to_v
                        if (c >= 0)
                            c_to_v[i][j] = c % 2;
                        else
                            c_to_v[i][j] = -1;
                    }
                }
            }
            // UPdate code_word
            for (int a = 0; a < n; a++)
            {
                int count_e = 0;
                int count_0 = 0;
                if (code_word[a] == 0)
                {
                    count_0++;
                }
                else
                {
                    count_e++;
                }

                for (int j = 0; j < u; j++)
                {

                    if (H[j][a] == 1 && c_to_v[j][a] == -1)
                    {
                        count_e++;
                    }
                    else if (H[j][a] == 1 && c_to_v[j][a] == 0)
                    {
                        count_0++;
                    }
                }

                if (count_0 == 0)
                {
                    Update_code[a] = -1;
                }
                else if (count_0 > 0)
                {
                    Update_code[a] = 0;
                }
            }
            // t>1 iteration vn_to_cn

            for (int t = 1; t <= 50; t++)
            {
                for (int i = 0; i < n; i++)
                {

                    for (int j = 0; j < u; j++)
                    {

                        if (H[j][i] == 1)
                        {

                            int count_e = 0;
                            int count_0 = 0;

                            if (code_word[i] == -1)
                            {
                                count_e++;
                            }
                            else
                            {
                                count_0++;
                            }

                            for (int k = 0; k < u; k++)
                            {

                                if (H[k][i] == 1 && c_to_v[k][i] == -1 && k != j) //flag
                                {
                                    count_e++;
                                }
                                else if (H[k][i] == 1 && c_to_v[k][i] == 0 && k != j) //flag
                                {
                                    count_0++;
                                }
                            }

                            if (count_0 == 0)
                            {
                                v_to_c[j][i] = -1;
                            }
                            else
                            {
                                v_to_c[j][i] = 0;
                            }
                        }
                    }
                }

                // t > 1 cn_to_vn

                for (int i = 0; i < u; i++)
                {

                    for (int j = 0; j < n; j++)
                    {

                        if (H[i][j] == 1)
                        {
                            int c = 0;
                            for (int k = 0; k < n; k++)
                            {
                                if (H[i][k] == 1 && k != j) //flag
                                {

                                    c += v_to_c[i][k];
                                }
                            }
                            // assign c_to_v
                            if (c >= 0)
                                c_to_v[i][j] = c % 2;
                            else
                                c_to_v[i][j] = -1;
                        }
                    }
                }

                for (int a = 0; a < n; a++)
                {
                    int count_e = 0;
                    int count_0 = 0;
                    if (code_word[a] == 0) //flag
                    {
                        count_0++;
                    }
                    else
                    {
                        count_e++;
                    }

                    for (int j = 0; j < u; j++)
                    {

                        if (H[j][a] == 1 && c_to_v[j][a] == -1)
                        {
                            count_e++;
                        }
                        else if (H[j][a] == 1 && c_to_v[j][a] == 0)
                        {
                            count_0++;
                        }
                    }

                    if (count_0 == 0)
                    {
                        Update_code[a] = -1;
                    }
                    else
                    {
                        Update_code[a] = 0;
                    }
                }
                int count_no_e = 0;
                for (int i = 0; i < n; i++)
                {

                    if (Update_code[i] == -1)
                    {
                        count_no_e++;
                    }
                }

                if (count_no_e == 0)
                {
                    prob_succ++;
                    itr = t;
                    break;
                }
            }
        }

        ans_graph[z] = (double)prob_succ / (double)1000;
        z++;
    }

    for (int i = 0; i < 25; i++)
    {
        printf("%lf ", ans_graph[i] * 100.00);
    }

    return 0;
}
