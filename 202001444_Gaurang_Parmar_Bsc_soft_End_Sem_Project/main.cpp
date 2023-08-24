#include<bits/stdc++.h>
#include<ctime>

using namespace std;
class bsc_decoding
{
private:
    map<int,map<int,double> > VN;

    map<int,map<int,double> > CN;

    int *r;
    int m,n;
    double *f;
    int **H;
public:
    void BSC(double );
    void simulator();
    bsc_decoding();
    void inp(int ,int );




};
bsc_decoding::bsc_decoding()
{
    r=NULL;
    f=NULL;
}
void bsc_decoding:: inp(int mt,int nt)
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
void bsc_decoding:: BSC(double p)
{

   // int *r=BSC(n);
    int i,j;
    static int count=1;                        //BSC(P) model
   // int mat[1000],ones;
    r=new int[n];
    f=new double[n];

        for(int i=0; i<n; i++)
        {
            float temp = (float)rand()/RAND_MAX;
            if(temp<p)
                {
                    r[i]=1;
                    f[i]=1-p;
                }
            else
                {
                    r[i]=0;
                    f[i]=p;
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

void bsc_decoding::simulator()
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
        //cout<<"OK";
            int ksim,Ncorr=0;
            for(ksim=1;ksim<=Nsim;ksim++)
            {
                //int no_er[100001]={0};
           // cout<<"Time to.."<<" "<<endl;
                  BSC(p);
             for(num=1;num<=125;num++)
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
                            sum = sum *(1-(2*ptr->second));
                            if(1-(2*ptr->second))
                            {
                                sum_nonzero*=(1-(2*ptr->second));
                            }
                            else
                                zeros++;

                        }
                       // cout<<"\n";
                        float k;
                        for(auto ptr=itr->second.begin();ptr!=itr->second.end();ptr++)
                        {
                           //cout<<"for vn "<<ptr->first<<"is "<<ptr->second<<" ";
                          if((1-(2*ptr->second)))
                          {
                              k = sum/(1-(2*ptr->second));

                          }
                          else
                          {
                              if(zeros==1)
                                k=sum_nonzero;
                              else
                                k=0;
                          }



                                VN[ptr->first][itr->first]= (1 - k)/2;

                        }


                    }


                for(auto itr=VN.begin();itr!=VN.end();itr++)
                {
                      // cout<<"VN recei " << itr->first<<":";
                      double sum=1,rsum=1,l,nsum=1,nrsum=1;
                        for(auto ptr=itr->second.begin();ptr!=itr->second.end();ptr++)
                        {
                          // cout<<"for cn "<<ptr->first<<" is "<<ptr->second<<" ";
                           sum = sum * ptr->second;
                           rsum = rsum*(1-ptr->second);
                        }
                        nsum=sum;
                        sum=sum*f[itr->first];
                        nrsum=rsum;
                        rsum=rsum*(1-f[itr->first]);
                        l= sum /rsum;
                        if(isnan(l))
                            l=0;

                        if(l<1)
                        {
                            //r[itr->first]=0;

                            r[itr->first]=0;
                             f[itr->first] = l/(l+1);
                           // cout<<0<<" ";
                        }
                        else
                        {

                           r[itr->first]=1;
                           f[itr->first] = l/(l+1);

                            //cout<<1<<" ";
                        }
                         nsum=nsum*f[itr->first];
                         nrsum=nrsum*(1-f[itr->first]);
                         l= nsum/nrsum;
                          if(isnan(l))
                            l=0;

                         for(auto ptr=itr->second.begin();ptr!=itr->second.end();ptr++)
                        {

                           double l1= (ptr->second)/(1-(ptr->second));
                           double once;
                           double lf;
                           if(isnan(l1))
                            l1=0;

                           lf = l/l1;
                           if(isnan(lf))
                            lf=0;

                           once = lf/(lf+1);
                           if(isnan(once))
                              once=0;


                                CN[ptr->first][itr->first]=once;


                        }


                }
                //cin>>x;
               /* if(brk==n )
                    break;*/


             }

           int ones=0;
            //add num_err after break
           for(int i=0;i<n;i++)
           {
               if(r[i]==1)
                ones++;
              // cout<<f[i]<<" ";
           }
           if(ones==0)
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
    bsc_decoding d;
    d.inp(m,n);



    d.simulator();

    return 0;
}
