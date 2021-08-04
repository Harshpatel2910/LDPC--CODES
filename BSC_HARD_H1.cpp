#include <bits/stdc++.h>

#include <fstream>
using namespace std;

int main()
{

    int n = 5056, u = 3792, itr = 0;

    int Update_code[n];

    // H matrix

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

    int wr, wc;
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
    int z = 0;
    double ans_graph[15];
    double w = 0.0;
    srand(time(0));
    for (int s = 0; s < 15; s++)

    {
        w += 0.02;

        int prob_succ = 0;
        for (int q = 1; q <= 10; q++)
        {

            // now we create BSC channel and generate code_word

            int code_word[n];

            for (int i = 0; i < n; i++)
            {

                double t = (double)rand() / (double)RAND_MAX;
                code_word[i] = (t < w);
            }

            //now we create one  2D array

            for (int i = 0; i < u; i++)
            {

                for (int j = 0; j < n; j++)
                {
                    v_to_c[i][j] = 0;
                    c_to_v[i][j] = 0;
                }
            }

            // now we perform decoding

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
                        c_to_v[i][j] = c % 2;
                    }
                }
            }
            // Update code_word
            for (int a = 0; a < n; a++)
            {
                int count_1 = 0;
                int count_0 = 0;
                if (code_word[a] == 0)
                {
                    count_0++;
                }
                else
                {
                    count_1++;
                }

                for (int j = 0; j < u; j++)
                {

                    if (H[j][a] == 1 && c_to_v[j][a] == 1)
                    {
                        count_1++;
                    }
                    else if (H[j][a] == 1 && c_to_v[j][a] == 0)
                    {
                        count_0++;
                    }
                }

                if (count_1 > count_0)
                {
                    Update_code[a] = 1;
                }
                else if (count_0 > count_1)
                {
                    Update_code[a] = 0;
                }

                else
                {
                    srand(time(0));
                    Update_code[a] = rand() % 2;
                }
            }

            for (int t = 1; t < 30; t++)
            {
                for (int i = 0; i < n; i++)
                {

                    for (int j = 0; j < u; j++)
                    {

                        if (H[j][i] == 1)
                        {

                            int count_1 = 0;
                            int count_0 = 0;

                            if (code_word[i] == 1)
                            {
                                count_1++;
                            }
                            else
                            {
                                count_0++;
                            }

                            for (int k = 0; k < u; k++)
                            {

                                if (H[k][i] == 1 && c_to_v[k][i] == 1 && k != j) //flag
                                {
                                    count_1++;
                                }
                                else if (H[k][i] == 1 && c_to_v[k][i] == 0 && k != j) //flag
                                {
                                    count_0++;
                                }
                            }

                            if (count_1 > count_0)
                            {
                                v_to_c[j][i] = 1;
                            }
                            else if (count_0 > count_1)
                            {
                                v_to_c[j][i] = 0;
                            }

                            else
                            {
                                srand(time(0));
                                v_to_c[j][i] = rand() % 2;
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
                            c_to_v[i][j] = c % 2;
                        }
                    }
                }

                for (int a = 0; a < n; a++)
                {
                    int count_1 = 0;
                    int count_0 = 0;
                    if (code_word[a] == 0) //flag
                    {
                        count_0++;
                    }
                    else
                    {
                        count_1++;
                    }

                    for (int j = 0; j < u; j++)
                    {

                        if (H[j][a] == 1 && c_to_v[j][a] == 1)
                        {
                            count_1++;
                        }
                        else if (H[j][a] == 1 && c_to_v[j][a] == 0)
                        {
                            count_0++;
                        }
                    }

                    if (count_1 > count_0)
                    {
                        Update_code[a] = 1;
                    }
                    else if (count_0 > count_1)
                    {
                        Update_code[a] = 0;
                    }

                    else
                    {
                        srand(time(0));
                        Update_code[a] = rand() % 2;
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
    for (int i = 0; i < 15; i++)
    {
        printf("%lf ", ans_graph[i]);
    }
    return 0;
}
