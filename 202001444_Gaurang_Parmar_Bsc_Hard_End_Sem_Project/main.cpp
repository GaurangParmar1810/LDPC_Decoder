#include<bits/stdc++.h>
#include<ctime>
#include<fstream>

using namespace std;
class bsc_decoding
{
private:
    map<int,map<int,int> > VN;

    map<int,map<int,int> > CN;

    int *r;
    int m,n;
   // int *f;
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
   // f=NULL;
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

    string s;
    string name;
     cout<<"Please enter name of text file of H matrix(with .txt extension): ";
    cin>>name;
    ifstream fin(name);
    if(!fin)
    {
        cout<<"Can't open file";
        exit(0);
    }
    int count=0;
    cout<<"OK"<<endl;
    while(getline(fin,s))
    {
       int t=0;

      for(i=0;i<s.length();i++)
      {
          if(s[i]!=',')
          {
              H[count][t]=s[i]-'0';
              //cout<<H[count][t]<<" ";
              if(H[count][t]==1)
            {
            CN[count][t]=-1;
            }
              t++;
          }
      }


       count++;
      // cout<<"\n";
    }
}
void bsc_decoding:: BSC(double p)
{

   // int *r=BSC(n);
    int i,j;
    static int count=1;                        //BSC(P) model
   // int mat[1000],ones;
    r=new int[n];


        for(int i=0; i<n; i++)
        {
            float temp = (float)rand()/RAND_MAX;
            if(temp<p)
                r[i]=1;
            else
                r[i]=0;
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
    int Nsim=100,num;
    double pcorrsim;
    int kk=0;
    srand((unsigned int)time(NULL));
     string output;
     cout<<"Please enter name of file (with .txt ): ";
     cin>>output;
    ofstream myfile(output);
    if(!myfile)
    {
        cout<<"Can't open file!";
        return ;
    }
   // cout<<"\n";

    for(p=0;p<=1.04;p=p+0.05)
    {
        //cout<<"OK";
            int ksim,Ncorr=0;
            for(ksim=1;ksim<=Nsim;ksim++)
            {
               // int no_er[100001]={0};
           // cout<<"Time to.."<<" "<<endl;
                  BSC(p);

              // int x;
             for(num=1;num<=1000;num++)
             {

                //VN->CN message passing
                    if(num==1)
                   {
                       int count=0;
                    for(auto itr=VN.begin();itr!=VN.end();itr++)
                    {
                      // cout<<"VN recei " << itr->first<<":";

                        for(auto ptr=itr->second.begin();ptr!=itr->second.end();ptr++)
                        {
                           // cout<<"for cn "<<ptr->first<<" is "<<ptr->second<<" ";
                           CN[ptr->first][itr->first]=r[count];


                        }
                        count++;

                    }
                   }

                   //cout<<"\n";
                //CN->VN Message passing
                    //int d=0;
                    for(auto itr=CN.begin();itr!=CN.end();itr++)
                    {
                        int sum=0;
                        int check;
                      // cout<<"CN recei " << itr->first<<":";
                        for(auto ptr=itr->second.begin();ptr!=itr->second.end();ptr++)
                        {
                           //cout<<"for vn "<<ptr->first<<"is "<<ptr->second<<" ";
                            sum = sum + ptr->second;

                        }
                        for(auto ptr=itr->second.begin();ptr!=itr->second.end();ptr++)
                        {
                           //cout<<"for vn "<<ptr->first<<"is "<<ptr->second<<" ";
                            check = sum - ptr->second;
                            if(check%2==0)
                            {
                                VN[ptr->first][itr->first]=0;
                            }
                            else
                            {
                                VN[ptr->first][itr->first]=1;
                            }


                        }

                     //  cout<<"\n";
                    }
                   //cout<<"\n";


               // cout<<d<<"d is"<<endl;
                   int brk=0;
                for(auto itr=VN.begin();itr!=VN.end();itr++)
                {
                      //cout<<"VN recei " << itr->first<<":";
                      int sum=0,count=1,nsum=1;
                        for(auto ptr=itr->second.begin();ptr!=itr->second.end();ptr++)
                        {
                          //cout<<"for cn "<<ptr->first<<" is "<<ptr->second<<" ";
                           sum = sum + ptr->second;
                           count++;
                        }
                       // no_er[num]=sum;
                        nsum=sum;
                        sum=sum+r[itr->first];
                        int zeros= count-sum;
                        if(zeros>=sum)
                        {
                            //r[itr->first]=0;
                            if(r[itr->first]==0)
                                brk++;
                            r[itr->first]=0;
                           // cout<<0<<" ";
                        }
                        else
                        {
                           // r[itr->first]=1;
                           if(r[itr->first]==1)
                                brk++;
                           r[itr->first]=1;

                            //cout<<1<<" ";
                        }

                      nsum=nsum+r[itr->first];
                      count--;
                         for(auto ptr=itr->second.begin();ptr!=itr->second.end();ptr++)
                        {

                           int once = nsum - ptr->second;
                           zeros = count-nsum;

                            if(zeros>=once)//in case p is known we can add that into condition as well
                            {
                                CN[ptr->first][itr->first]=0;
                            }
                            else
                            {
                               CN[ptr->first][itr->first]=1;
                            }
                        }

                    // cout<<"\n";
                }
                 if(brk==n)
                    break;
                // cin>>x;


             }


          // cout<<num<<endl;
           int ones=0;
            //add num_err after break
           for(int i=0;i<n;i++)
           {
               if(r[i]==1)
                ones++;
               //cout<<r[i]<<" ";
           }
           //cout<<"\n";
           if(ones==0)
           {
               Ncorr++;
           }
            cout<<ksim<<"\r";
           delete[] r;

         // cout<<"Ok"<<endl;
       }

          pcorrsim= (Ncorr*1.0)/Nsim;

           cout<<"p is "<<p<<"s rate "<<pcorrsim<<endl;
           myfile<<pcorrsim<<"\n";
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
