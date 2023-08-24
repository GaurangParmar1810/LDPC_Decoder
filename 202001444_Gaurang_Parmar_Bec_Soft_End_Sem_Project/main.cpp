#include<bits/stdc++.h>
#include<ctime>

using namespace std;
class bec_decoding
{
private:
    map<int,map<int,double> > VN;

    map<int,map<int,double> > CN;

    int *r;
    int *r_o;
    int m,n;
    double *f;
    int **H;
public:
    void BEC(double );
    void simulator();
    bec_decoding();
    void inp(int ,int );




};
bec_decoding::bec_decoding()
{
    r=NULL;
    f=NULL;
    r_o=NULL;
}
void bec_decoding:: inp(int mt,int nt)
{
     m=mt;
    n=nt;
    int i,j;

     H = new int*[m];
    for(i = 0; i < m; ++i) {
     H[i] = new int[n];
    }

    string inp;
    string s;
     cout<<"Please enter name of text file of H matrix(with .txt extension): ";
    cin>>inp;
    ifstream fin(inp);
    if(!fin)
    {
        cout<<"Can't open file";
        exit(1);
    }
    int count=0;
    cout<<"OK"<<endl;
    while(getline(fin,s))
    {
       int t=0;
       //check=0;
      for(i=0;i<s.length();i++)
      {

          if(s[i]!=',')
          {
              //check++;
              H[count][t]=s[i]-'0';
              //cout<<H[count][t]<<endl;
              if(H[count][t]==1)
            {
            CN[count][t]=-1;
            }
              t++;
          }

      }
      //cout<<check<<endl;

       count++;
    }
    //cout<<count<<endl;
}
void bec_decoding:: BEC(double p)
{

    int i,j;
    static int count=1;                        //BEC(P) model
   // int mat[1000],ones;
    r=new int[n];
    r_o=new int[n];

    f=new double[n];

        for(int i=0; i<n; i++)
        {
            float temp = (float)rand()/RAND_MAX;
            if(temp<p)
                {
                    r[i]=-1;//erased message
                    r_o[i]=-1;
                    f[i]=0;
                }
            else
                {
                    r[i]=0;
                    r_o[i]=0;
                    f[i]=1e25;
                }
        }


    //just to maintain the steps

    if(count==1)
    {
         for(j=0;j<n;j++)
      {
        for(i=0;i<m;i++)
        {
            if(H[i][j]==1)
            {

               VN[j][i]=r[j];
            }
        }
      }
    }
     count++;
}

void bec_decoding::simulator()
{
    double p;
    int Nsim=500,num;
    double pcorrsim;
    int kk=0;
    srand(time(0));
    string output;
     cout<<"Please enter name of file (with .txt ): ";
     cin>>output;
    ofstream myfile(output);
    if(!myfile)
    {
        cout<<"Can't open file!";
        return ;
    }
    for(p=0;p<=1.02;p=p+0.05)
    {

            int ksim,Ncorr=0;
            for(ksim=1;ksim<=Nsim;ksim++)
            {

           // cout<<"Time to.."<<" "<<endl;
                  BEC(p);
             for(num=1;num<=200;num++)
             {

                //VN->CN message passing
               // int x;
                    if(num==1)
                   {
                       int count=0;
                    for(auto itr=VN.begin();itr!=VN.end();itr++)
                    {
                      /* cout<<"VN recei " << itr->first<<":";*/

                        for(auto ptr=itr->second.begin();ptr!=itr->second.end();ptr++)
                        {
                           // cout<<"for cn "<<ptr->first<<" is "<<ptr->second<<" ";

                           CN[ptr->first][itr->first]=f[count];

                        }
                        count++;

                    }
                   }

                   //cout<<"\n";
                //CN->VN Message passing
                    //int d=0;
                    for(auto itr=CN.begin();itr!=CN.end();itr++)
                    {
                         double sum=1,sum_nonzero=1,zeros=0;
                     //  cout<<"CN recei " << itr->first<<":";
                        for(auto ptr=itr->second.begin();ptr!=itr->second.end();ptr++)
                        {
                          // cout<<"for vn "<<ptr->first<<"is "<<ptr->second<<" ";
                            sum = sum *(tanh((ptr->second)/2.0));
                            if(tanh((ptr->second)/2.0))
                            {
                                sum_nonzero*=(tanh((ptr->second)/2.0));
                            }
                            else
                                zeros++;

                        }
                       // cout<<"\n";
                        float k;
                        for(auto ptr=itr->second.begin();ptr!=itr->second.end();ptr++)
                        {
                           //cout<<"for vn "<<ptr->first<<"is "<<ptr->second<<" ";
                           if(tanh((ptr->second)/2.0))
                          {
                              k = sum/(tanh((ptr->second)/2.0));
                          }
                          else
                          {
                              if(zeros==1)
                                k=sum_nonzero;
                              else
                                k=0;
                          }
                            if(k==1)
                                VN[ptr->first][itr->first]=2*(1e25);
                            else if(k==-1)
                            {
                                VN[ptr->first][itr->first]=-1*2*(1e25);
                            }
                            else
                            {
                                VN[ptr->first][itr->first]=2*atanh(k);
                            }
                        }
                    }
                    //cout<<"\n";
                for(auto itr=VN.begin();itr!=VN.end();itr++)
                {
                      // cout<<"VN recei " << itr->first<<":";
                      double sum=0,nsum=0;

                        for(auto ptr=itr->second.begin();ptr!=itr->second.end();ptr++)
                        {
                          // cout<<"for cn "<<ptr->first<<" is "<<ptr->second<<" ";
                           sum = sum + ptr->second;
                        }
                       // cout<<sum<<"|";
                        nsum=sum;
                        sum=sum+f[itr->first];
                         f[itr->first] = sum;//2nd change
                        if(sum<0)
                        {
                            if(r_o[itr->first]==-1)
                               {
                                   r[itr->first]=1;

                               }
                        }
                        else if(sum==0)
                        {

                             ;
                        }
                        else
                        {
                            if(r_o[itr->first]==-1)//1st change
                            {
                                r[itr->first]=0;

                            }

                        }
                       nsum=nsum+f[itr->first];
                         for(auto ptr=itr->second.begin();ptr!=itr->second.end();ptr++)
                        {
                           CN[ptr->first][itr->first]=nsum - (ptr->second);


                        }



                }
                //cin>>x;
               /* if(brk==n )
                    break;*/


             }

           int zeros=0;
            //add num_err after break
           for(int i=0;i<n;i++)
           {
               if(r[i]==0)
                zeros++;
              // cout<<f[i]<<" ";
           }
           if(zeros==n)
           {
               Ncorr++;
           }
           cout<<ksim<<"\r";
           delete[] r;
           delete[] f;

          //cout<<"Ok"<<endl;
       }

          pcorrsim= (Ncorr*1.0)/Nsim;
          myfile<<pcorrsim<<"\n";

           cout<<"p is "<<p<<"s rate "<<pcorrsim<<endl;
           kk++;
    }

     myfile.close();
}
int main()
{
    cout<<"Enter dimention of H matrix: ";
    int m,n;
    cin>>m>>n;
    bec_decoding d;
    d.inp(m,n);



    d.simulator();

    return 0;
}
