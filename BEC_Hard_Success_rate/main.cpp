#include<bits/stdc++.h>
#include<ctime>

using namespace std;
class bec_decoding
{
private:
    map<int,map<int,int> > VN;

    map<int,map<int,int> > CN;

    int *r;
    int m,n;
   // int *f;
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
   // f=NULL;
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

    string s;
    string name;
     cout<<"Please enter name of text file of H matrix(with .txt extension): ";
    cin>>name;
    ifstream fin(name);
    if(!fin)
    {
        cout<<"Can't open file";
        return ;
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
              if(H[count][t]==1)
            {
            CN[count][t]=-1;
            }
              t++;
          }
      }


       count++;
    }
}
void bec_decoding:: BEC(double p)
{

   // int *r=BEC(n);
    int i,j;
    static int count=1;                        //BEC(P) model
   // int mat[1000],ones;
    r=new int[n];


        for(int i=0; i<n; i++)
        {
            float temp = (float)rand()/RAND_MAX;
            if(temp<p)
                r[i]=-1;//here -1 represents an erasure
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

void bec_decoding::simulator()
{
    double p;
    int Nsim=100,num;
    double pcorrsim[101];
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

           // cout<<"Time to.."<<" "<<endl;
                  BEC(p);
             for(num=1;num<=1000;num++)
             {

                //VN->CN message passing
                    if(num==1)
                   {
                       int count=0;
                    for(auto itr=VN.begin();itr!=VN.end();itr++)
                    {
                      /* cout<<"VN recei " << itr->first<<":";*/

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
                        int erasure=0;
                       //cout<<"CN recei " << itr->first<<":";
                        for(auto ptr=itr->second.begin();ptr!=itr->second.end();ptr++)
                        {
                           //cout<<"for vn "<<ptr->first<<"is "<<ptr->second<<" ";
                           if(ptr->second!=-1)
                            sum = sum + ptr->second;
                           else
                                erasure++;

                        }
                        for(auto ptr=itr->second.begin();ptr!=itr->second.end();ptr++)
                        {
                            int other_e=erasure;
                           //cout<<"for vn "<<ptr->first<<"is "<<ptr->second<<" ";
                          if(ptr->second==-1)
                          {
                              other_e--;
                              check=sum;
                          }
                          else
                          {
                               check = sum - ptr->second;
                          }


                          if(other_e==0)
                          {
                            if((check%2)!=0)
                            {
                                VN[ptr->first][itr->first]=1;
                            }
                            else
                            {
                                VN[ptr->first][itr->first]=0;
                            }
                          }
                          else
                          {
                              VN[ptr->first][itr->first]=-1;
                          }


                        }


                    }
                  // cout<<"\n";
                    /*if(count%2==0)
                    {
                        d++;
                    }*/

               // cout<<d<<"d is"<<endl;
                   int brk=0;
                for(auto itr=VN.begin();itr!=VN.end();itr++)
                {
                      /* cout<<"VN recei " << itr->first<<":";*/
                      int sum=0,count=1,nsum=1,nerasure=0;
                      int erasure=0;
                        for(auto ptr=itr->second.begin();ptr!=itr->second.end();ptr++)
                        {
                           // cout<<"for cn "<<ptr->first<<" is "<<ptr->second<<" ";
                           if(ptr->second!=-1)
                           sum = sum + ptr->second;
                           else
                            erasure++;

                           count++;
                        }
                       // no_er[num]=sum;
                       nsum=sum;
                       nerasure=erasure;
                        if(r[itr->first]!=-1)
                        sum=sum+r[itr->first];
                        else
                            erasure++;

                        if(erasure==count)
                        {
                            //r[itr->first]=0;
                            if(r[itr->first]==-1)
                                brk++;
                            r[itr->first]=-1;
                           // cout<<0<<" ";
                        }
                        else
                        {
                           // r[itr->first]=1;
                           if(sum!=0)
                           {
                               if(r[itr->first]==1)
                                brk++;
                              r[itr->first]=1;

                           }
                           else
                           {
                               if(r[itr->first]==0)
                                brk++;
                               r[itr->first]=0;

                           }
                            //cout<<1<<" ";
                        }
                         if(r[itr->first]!=-1)
                        nsum=nsum+r[itr->first];
                        else
                            nerasure++;




                    /* cout<<":";
                      cin>>z;*/
                      count--;
                         for(auto ptr=itr->second.begin();ptr!=itr->second.end();ptr++)
                        {

                           int r_erasure=nerasure,once;
                           if(ptr->second==-1)
                           {
                               r_erasure--;
                           }
                           else
                           {
                               once = nsum - ptr->second;
                           }
                           if(r_erasure==count)
                           {
                               CN[ptr->first][itr->first]=-1;
                           }
                           else
                           {
                               if(once==0)//
                            {
                                CN[ptr->first][itr->first]=0;
                            }
                            else
                            {
                               CN[ptr->first][itr->first]=1;
                            }
                           }




                        }


                }
                 if(brk==n)
                    break;


             }


          // cout<<num<<endl;
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

          cout<<"Ok"<<endl;
       }

          pcorrsim[kk]= (Ncorr*1.0)/Nsim;
           myfile<<pcorrsim[kk]<<"\n";

           cout<<"p is "<<p<<"s rate "<<pcorrsim[kk]<<endl;
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

