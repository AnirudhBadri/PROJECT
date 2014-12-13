#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <math.h>
#include <windows.h>
using namespace std;
#define DEBUG 0

class player
{
    public:
    char name[40],pos[4];
    int overall,pace,shoot,pass,dribble,defence,physical,initialIndex;

}initialTeam[8][17],finalTeam[8][11];
class matchDetails
{
    public:
    int teamOneGoal,teamTwoGoal,teamOneIndex,teamTwoIndex;
}matchNumber[28];
//****************GLOBAL FUNCTION DECLARATIONS***************
int goalPlayer(int team);
void initialiseArray();
void chooseTeam();
void simulateLeague();
void bookings(player statBayern[][11],int x,int y);
void gameTimeLiveDisplay(int u,int v,int x);
float formula(player statBarca[][11],int m,int x);
int posessionTeamOne(int prev);
void populate(player statBayern[][17],ifstream &bayern,int i);
void initialisePlayers();
int goalRange(float ratio);
void userTeam(player originalTeam[][17],player finalLineup[][11],int m,int temp);
void displayTeamDetails(player statBayern[][17],int x);          //Displays full team details which allows for further player stats
void displyPlayerDetails(player statBayern[][17],int s,int x);
int bookingPlayer(int team, int y, int x);
void buildLineup(player statBayern[][17],player finalTeam[][11],int i);
void cleanStartTeams();
//*************END OF GLOBAL FUNCTION DECLARATIONS***********

int goalOne[7][2], goalTwo[7][2],onTargetOne=0,onTargetTwo=0,bookOne[8][2],bookTwo[8][2],points[8],user,temp,fileCounter=0;
int teamSelect[8][17];
char teamOneName[40],teamTwoName[40];
char teamName[][40]={"Real Madrid","Manchester United","Barcelona","Atletico Madrid","Bayern Munich","PSG","Chelsea","Manchester City"};
//************FUNCTION DEFINITIONS***************************
void populate(player statBayern[][17],ifstream &bayern,int x)
{
    int i=0;
    for(i=0;i<17;i++)
    {
        bayern.getline(statBayern[x][i].name,40,'\t');
        bayern>>statBayern[x][i].overall;
        bayern>>statBayern[x][i].pos;
        bayern>>statBayern[x][i].pace;
        bayern>>statBayern[x][i].shoot;
        bayern>>statBayern[x][i].pass;
        bayern>>statBayern[x][i].dribble;
        bayern>>statBayern[x][i].defence;
        bayern>>statBayern[x][i].physical;
        bayern.ignore();

        if(DEBUG==1)
        {
            puts(statBayern[x][i].name);
            cout<<statBayern[x][i].overall<<endl;
            puts(statBayern[x][i].pos);
            cout<<statBayern[x][i].pace<<endl;
            cout<<statBayern[x][i].shoot<<endl;
            cout<<statBayern[x][i].pass<<endl;
            cout<<statBayern[x][i].dribble<<endl;
            cout<<statBayern[x][i].physical<<endl;
            cout<<statBayern[x][i].defence<<endl;
        }
    }
}
float formula(player statBarca[][11],int m,int x)                 //Returns a float for attack/defence stats
{
    float shootAttack=0,paceAttack=0,physicalAttack=0;
    float paceDefence=0,shootDefence=0,physicalDefence=0;
    float pass=0,dribble=0,defend=0;
    int i=0;
    for(i=1;i<11;i++)
    {
        if(m==0&&i>4)
        {
            paceAttack = paceAttack + statBarca[x][i].pace;
            shootAttack = shootAttack + statBarca[x][i].shoot;
            physicalAttack = physicalAttack + statBarca[x][i].physical;
            dribble = dribble + statBarca[x][i].dribble;
        }
        else if(m==1&&i<8)
        {
            paceDefence = paceDefence + statBarca[x][i].pace;
            shootDefence = shootDefence + statBarca[x][i].shoot;
            physicalDefence = physicalDefence + statBarca[x][i].physical;
            defend = defend + statBarca[x][i].defence;
            pass = pass + statBarca[x][i].pass;
        }
        if(DEBUG==1)
        {
        //    cout<<pace<<" "<<shoot<<" "<<dribble<<" "<<physical<<" "<<defend<<" "<<pass<<"\n";
        }
    }
    paceAttack = paceAttack/7;
    shootAttack = shootAttack/7;
    paceDefence = paceDefence/7;
    shootDefence = shootDefence/7;
    physicalAttack = physicalAttack/7;
    dribble = dribble/7;
    physicalDefence = physicalDefence/7;
    defend = defend/7;
    pass= pass/7;
    float clearance=(shootDefence + pass)/2;
    if(m==0)
        return 0.2*(physicalAttack) + 0.5*(paceAttack) + 0.7*(dribble) + 0.9*(shootAttack);   //ATTACK
    if(m==1)
        return 0.2*(clearance) + 0.5*(paceDefence) + 0.7*(physicalDefence) + 0.9*(defend);  //DEFENCE
}
int goalRange(float ratio)                              //Uses formula() to generate a goal range
{
    int goals;
    //for(i=0;i<10;i++)
    {   if(ratio<0.85)
            goals=rand()%2;
        else if(0.85<=ratio && ratio<0.9)
            goals=rand()%2;
        else if(0.9<=ratio && ratio<0.95)
            goals=rand()&3;
        else if(0.95<=ratio && ratio<1)
            goals=rand()%3+rand()%3;
        else if(1<=ratio)// && ratio<0.66)
            goals=rand()%4+ 2+rand()%2;
        else
            cout<<"Issue Here";
        if (goals<0)
            goals=0;
    }
    return goals;
}
int bookings(int x)                                     //Returns number of yellow cards per match
{
    int i,cards=0,phy=0;
    for(i=1;i<11;i++)
       phy+=finalTeam[x][i].physical;
    phy = phy/10;
    if (phy>=80)
    cards = rand()%3 + 3;
    else if((phy>=70)&&(phy<80))
    cards = rand()%3 + 1;
    else
    cards = rand()%2;
    return cards;
}
void bookTime(int totBook, int teamNo, int u, int v)    //Returns time of yellow card
{
    int i,time,bookingTime=0;
    for(i=0;i<totBook;)
    {
        time = rand()%90 + 1;
        if(teamNo==1)
        {
            bookOne[i][0]= time;
            bookOne[i][1]= bookingPlayer(teamNo,u,v);
            i++;
        }
        else if(teamNo==2)
        {
            bookTwo[i][0]= time;
            bookTwo[i][1]= bookingPlayer(teamNo,u,v);
            i++;
        }
    }
}
int bookingPlayer(int team,int u, int v)                //returns player index who commits foul
{
    int i,p;
    while(1)
    {
        i = rand()%10 + 1;
        if(team==1)
        {
            while(bookOne[i][0]>0)
                i = rand()%10 + 1;
            if ((i>4 && i<=7))
            {
                p = rand()%3;
                if(p==1)
                    return i;
            }
            else if (i>7)
            {
                p = rand()%9;
                if(p==1)
                    return i;
            }
            else
            {
                p = rand()%15;
                if(p==1)
                {
                    return i;
                }
            }
        }
        else
        {
            while (bookTwo[i][0]>0)
                i = rand()%10 + 1;
            if ((i>4 && i<=7))
            {
                p = rand()%3;
                if(p==1)
                    return i;
            }
            else if (i>7)
            {
                p = rand()%9;
                if(p==1)
                    return i;
            }
            else
            {
                p = rand()%15;
                if(p==1)
                {
                    return i;
                }
            }
        }

    }
}
void displayTeamDetails(player statBayern[][17],int x)            //Displays full team details which allows for further player stats
{
    system("cls");
    int i,m=0;
    char test;
    while(test!='l')
    {
        system("cls");
        cout<<"\t\t";
        for(i=0;i<17;i++)
        {
            if(i==m)
                cout<<'>';
            cout<<statBayern[x][i].name<<"\n\t\t";
        }
        test=getch();
        if((test=='d'||test=='s')&&m<17)
            m++;
        if((test=='a'||test=='w')&&m>0)
            m--;
                if(test=='k')
                chooseTeam();

    }
    displyPlayerDetails(statBayern,m,x);
}
void displyPlayerDetails(player statBayern[][17],int s,int x)     //Displays player details, accessed through displayTeamDetails()
{
    system("cls");
    char test;
    while(test!='l')
    {
        system("cls");
        cout<<"\t\t\t";
        cout<<"Player Name:\t";
        puts(statBayern[x][s].name);
        cout<<"\n\t\t";
        cout<<"OVERALL\t\t\t\tPOSITION\n\t\t";
        cout<<statBayern[x][s].overall<<"\t";
        cout<<"\t\t\t";
        puts(statBayern[x][s].pos);
        cout<<"\t\tPACE\t\tSHOOTING\tPASSING\n\t\t";
        cout<<statBayern[x][s].pace<<"\t\t";
        cout<<statBayern[x][s].shoot<<"\t\t";
        cout<<statBayern[x][s].pass<<endl;
        cout<<"\t\tDRIBBLING\tPHYSICAL\tDEFENCE\n\t\t";
        cout<<statBayern[x][s].dribble<<"\t\t";
        cout<<statBayern[x][s].physical<<"\t\t";
        cout<<statBayern[x][s].defence<<endl;
        test=getch();
        if(test=='k')
            displayTeamDetails(statBayern,x);
    }
    displayTeamDetails(statBayern,x);
}
void chooseTeam()                                        //User chooses custom team
{
    int i,m=0;
    char test;
    char teamName[][40]={{"Real Madrid"},{"Manchester United"},{"Barcelona"},{"Atletico Madrid"},{"Bayern Munich"},{"PSG"},{"Chelsea "},{"Manchester City"}};
    while(test!='l')
    {
        system("cls");
        cout<<"\n\n\t\t\t\tChoose your Team\n\n";
        for(i=0;i<8;i++)
        {

            cout<<"\t\t";
            if(i==m)
                cout<<'>';
            cout<<teamName[i]<<"\t\t";
            i++;
            if(i==m)
                cout<<'>';
            cout<<teamName[i]<<"\n";
        }
        test=getch();
        if(test=='d'&&m<7)
            m++;
        if(test=='a'&&m>0)
            m--;
        if(test=='s'&&m<6)
            m+=2;
        if(test=='w'&&m>1)
            m-=2;
        if(test==(char)13)
            break;
    }
    system("cls");
    //if(test=='p')
    {
    //    userTeam(initialTeam,finalTeam,m);
        test='l';
    }
    user=m;
}
void playAllMatches()                                    //define match order
{
    int i,j,count=0;
    for(i=0;i<7;i++)
    {
        for(j=i+1;j<8;j++)
        {
            matchNumber[count].teamOneIndex=i;
            matchNumber[count].teamTwoIndex=j;
            count++;
        }
    }
}
void simulateLeague()                                   //Run league simu;ation
{
    bool test[28],flag=0;
    int temp,i,m;
    char sample='l';
    for(i=0;i<28;i++)
        test[i]=0;
    for(i=0;i<28;i++)
    {
        flag=0;
        cleanStartTeams();
        temp=rand()%28;
        while(test[temp]!=0)
            temp=rand()%28;
        if(matchNumber[temp].teamOneIndex==user||matchNumber[temp].teamTwoIndex==user)
        {
            system("cls");
            cout<<"\n\n\n\n\t\t"<<teamName[matchNumber[temp].teamOneIndex]<<" vs "<<teamName[matchNumber[temp].teamTwoIndex]<<endl;
            Sleep(1800);
            system("cls");
            userTeam(initialTeam,finalTeam,user,temp);
            flag=1;

        }
        /*if(flag==0)
        {
            while(sample!='p')
            {
                cout<<"\n\n\t\t"<<teamName[matchNumber[temp].teamOneIndex]<<" vs "<<teamName[matchNumber[temp].teamTwoIndex]<<endl;
                cout<<"What do you want to do?"<<endl;
                if(m==0)
                    cout<<'>';
                cout<<"Watch match simulation"<<endl<<endl;
                if(m==1)
                    cout<<'>';
                cout<<"Skip match";
                sample=getch();
                if(sample=='w'&&m==1)
                    m--;
                if(sample=='s'&&m==0)
                    m++;
            }
        }*/
        gameTimeLiveDisplay(matchNumber[temp].teamOneIndex,matchNumber[temp].teamTwoIndex,0);
        getch();
        test[temp]=1;
    }
}
void cleanStartTeams()                                  //Build final 11 man team lists for all teams including custom player team
{
    int i;
    for(i=0;i<8;i++)
        buildLineup(initialTeam,finalTeam,i);
}
void initialisePlayers()                                //File Handling, taking data from file and putting it in variables
{
    ifstream bayern,chelsea,madrid,barca,psg,city,united,atletico;
    bayern.open("Bayern.txt",ios::in);
    chelsea.open("Chelsea.txt",ios::in);
    madrid.open("Madrid.txt",ios::in);
    barca.open("Barca.txt",ios::in);
    psg.open("PSG.txt",ios::in);
    city.open("City.txt",ios::in);
    united.open("United.txt",ios::in);
    atletico.open("Atletico.txt",ios::in);
    populate(initialTeam,bayern,4);
    populate(initialTeam,chelsea,6);
    populate(initialTeam,madrid,0);
    populate(initialTeam,united,1);
    populate(initialTeam,atletico,3);
    populate(initialTeam,barca,2);
    populate(initialTeam,city,7);
    populate(initialTeam,psg,5);
}
void game()                                             //Handles stuff
{
    initialisePlayers();
    initialiseArray();
    int q;
    ofstream match;
    match.open("MatchSummary.txt",ios::out);
    match.close();
    cleanStartTeams();
    playAllMatches();
    chooseTeam();
    simulateLeague();
    ofstream match2;
    match2.open("MatchSummary.txt",ios::out|ios::app);
    match2<<"\n\n\t\tPoints Table"<<endl;
    for(q=0;q<8;q++)
        match2<<"\t"<<points[q]<<" \t"<<teamName[q]<<endl;
    getch();
    int max=points[0],index=0;
    for(q=0;q<8;q++)
    {
        if(points[q]>max)
        {
            max=points[q];
            index=q;
        }
    }
    system("cls");
    cout<<"\n\n\n\t\t\tThe winner is\n\t\t\t\t"<<puts(teamName[q])<<"!!";
}
void goalTime(int goals, int teamNo)                    //Returns time of goal
{
    int i,time,goalTime=0;
    for(i=0;i<goals;)
    {
        time = rand()%90 + 1;
        if(teamNo==1)            //Used globally declared array to store the goal times
        {
            goalOne[i][0]= time;
            goalOne[i][1]= goalPlayer(1);
            i++;
        }
        else if(teamNo==2)
        {
            goalTwo[i][0]= time;
            goalTwo[i][1]= goalPlayer(2);
            i++;
        }
    }
}
void initialiseArray()                                  //Initialises user team condition array
{
    int i=8,j=17;
    for(i=0;i<8;i++)
    {
        for(j=0;j<17;j++)
        {
            teamSelect[i][j]=0;
        }
    }
}
void assignPoints(int i, int j,int u,int v)             //Assigns points after match, 3 for win, 1 for draw and 0 for loss
{
    if(i>j)
        points[u]+=3;
    else if(j>i)
        points[v]+=3;
    else
    {
        points[u]++;
        points[v]++;
    }
}
void buildLineup(player statBayern[][17],player finalTeam[][11],int x)              //Compile final random opponent teams
{
    bool test[17]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int count=0,i,temp;
    for(i=1;;i++)
    {
        temp=rand()%16+1;
        if(test[temp]==0)
        {
            test[temp]=1;
            count++;
        }
        if(count==10)
            break;
    }
    count=1;
    finalTeam[x][0]=statBayern[x][0];
    for(i=1;count<11;i++)
    {
        if(test[i]==1)
        {
            finalTeam[x][count]=statBayern[x][i];
            count++;
        }
    }
}
void userTeam(player originalTeam[][17],player finalTeam[][11],int x,int temp)      //User chooses squad
{
    bool test[17]={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int count=0,m=1,i;
    char ch;
    while(count<10)
    {
        system("cls");
        cout<<"\n\n\t\t"<<teamName[matchNumber[temp].teamOneIndex]<<" vs "<<teamName[matchNumber[temp].teamTwoIndex]<<endl;
        cout<<"\n\n\t\t";
        for(i=0;i<17&&count<11;i++)
        {
            if(i==m)
                cout<<'>';
            if(test[i]==1)
                cout<<"   >";
            cout<<originalTeam[x][i].name<<"\n\t\t";
        }

        ch=getch();
        if((ch=='d'||ch=='s')&&m<16)
            m++;
        if((ch=='a'||ch=='w')&&m>1)
            m--;
        if(ch==(char)13&&test[m]!=1)
        {
            test[m]=1;
            count++;
        }
    }
    system("cls");
    count=1;
    finalTeam[x][0]=originalTeam[x][0];
    for(i=1;count<11;i++)
    {
        if(test[i]==1)
        {
            finalTeam[x][count]=originalTeam[x][i];
            finalTeam[x][count].initialIndex=i;
            count++;
        }
    }
    system("cls");
}
void substitutions(int x)                               //Incomplete substitutions function
{
    int i,j,count=0,temp,k,m=0,flag=-1,flag2;
    char ch;
    char test;
    while(flag==-1)
    {
        for(i=0;i<17&&count<11;i++)
        {
            if(i==m)
                cout<<'>';
            if(teamSelect[x][i]==1)
                cout<<"   >";
            cout<<initialTeam[x][i].name<<"\n\t\t";
        }

        ch=getch();
        if((ch=='d'||ch=='s')&&m<16)
            m++;
        if((ch=='a'||ch=='w')&&m>1)
            m--;
        if(ch==(char)13&&teamSelect[x][m]==1)
        {
            teamSelect[x][m]=0;
            flag=m;
            count++;
        }
    }
    flag=0;
    while(flag2==0)
    {
        for(i=0;i<17&&count<11;i++)
        {
            if(i==m)
                cout<<'>';
            if(teamSelect[x][i]==1)
                cout<<"   >";
            cout<<initialTeam[x][i].name<<"\n\t\t";
        }

        ch=getch();
        if((ch=='d'||ch=='s')&&m<16)
            m++;
        if((ch=='a'||ch=='w')&&m>1)
            m--;
        if(ch==(char)13&&teamSelect[x][m]==1)
        {
            teamSelect[x][m]=0;
            finalTeam[x][flag]=finalTeam[x][m];
            flag2=1;
            count++;
        }
    }
}
int goalPlayer(int team)                                //Returns index of scoring player
{
    int i,p;
    while(1)
    {
        i = rand()%10 + 1;
        if (i>7)
        {
            if(team==1)
                onTargetOne++;
            else
                onTargetTwo++;
            p = rand()%3;
            if(p==1)
            {
                return i;
            }
        }
        else if (i>4 && i<=7)
        {
            p = rand()%9;
            if(team==1)
                onTargetOne++;
            else
                onTargetTwo++;
            if(p==1)
                return i;
        }
        else
        {
            p = rand()%15;
            if(team==1)
                onTargetOne++;
            else
                onTargetTwo++;
            if(p==1)
            {
                return i;
            }
        }
    }
}
int posessionTeamOne(int prev)                          //Returns posession
{
    int c=prev+pow((-1),rand()%2)*2;
    if(c<35)
        return prev +2;
    if(c>65)
        return prev-2;
    else
        return c;
}
void gameTimeLiveDisplay(int u,int v,int x)             //Display on console
{
    int i,j,k,l,y,q,m,n,countOne=0,countTwo=0,z;
    char c;
    ofstream match;
    match.open("MatchSummary.txt",ios::out|ios::app);
    strcpy(teamOneName,teamName[u]);
    strcpy(teamTwoName,teamName[v]);
    i=goalRange(formula(finalTeam,0,u)/formula(finalTeam,1,v));
    j=goalRange(formula(finalTeam,0,v)/formula(finalTeam,1,u));
    m=bookings(u);
    n=bookings(v);
    bookTime(m,1,u,v);
    bookTime(n,2,u,v);
    assignPoints(i,j,u,v);
    goalTime(i,1);
    goalTime(j,2);
    //cout<<i<<" - "<<j<<"\n";
    if(x==0)
    {
        for(k=0;k<91;k++)
        {
            system("cls");
            //cout<<i<<" - "<<j<<"\n";
            cout<<"Match Number : "<<fileCounter+1<<"\n\n\t\t\t"<<teamOneName<<" "<<countOne<<" - "<<countTwo<<" "<<teamTwoName<<endl;
            cout<<"\n\t\t\t\t"<<k<<endl;
            for(z=0;z<k;z++)
            {
                for(y=0;y<i;y++)
                {
                    if(goalOne[y][0]==z)
                    {
                        cout<<"\t"<<z<<"' "<<finalTeam[u][goalOne[y][1]].name<<" scores for "<<teamOneName<<"!"<<endl;
                    }
                }
                for(y=0;y<j;y++)
                {
                    if(goalTwo[y][0]==z)
                    {
                        cout<<"\t"<<z<<"' "<<finalTeam[v][goalTwo[y][1]].name<<" scores for "<<teamTwoName<<"!"<<endl;
                    }
                }
                for(y=0;y<m;y++)
                {
                    if(bookOne[y][0]==z)
                    {
                        cout<<"\t"<<z<<"'"<<"\t"<<"Yellow Card! "<<finalTeam[u][bookOne[y][1]].name<<"!"<<teamOneName<<"!"<<endl;
                    }
                }
                for(y=0;y<n;y++)
                {
                    if(bookTwo[y][0]==z)
                    {
                        cout<<"\t"<<z<<"'"<<"\t"<<"Yellow Card! "<<finalTeam[v][bookOne[y][1]].name<<"!"<<teamTwoName<<"!"<<endl;
                    }
                }
            }
            for(l=0;l<i;l++)
            {
                if(goalOne[l][0]==k)
                 {
                    cout<<"\t\t"<<k<<"' "<<finalTeam[u][goalOne[l][1]].name<<" scores for "<<teamOneName<<"@"<<endl;
                    countOne++;
                 }
            }
            for(l=0;l<j;l++)
            {
                if(goalTwo[l][0]==k)
                {
                    cout<<"\t\t"<<k<<"' "<<finalTeam[v][goalTwo[l][1]].name<<" scores for "<<teamTwoName<<"!"<<endl;
                    countTwo++;
                }
            }
            /*if(kbhit())
            {
                c=getch();
                if(c=='u')
                {
                    substitutions(user);
                }
            }
            */
            Sleep(25);
        }
    }
    if(onTargetOne==0)
        onTargetOne=rand()%7+1;
    if(onTargetTwo==0)
        onTargetTwo=rand()%7+1;
    while(onTargetOne>22)
        onTargetOne-=(rand()%5+6);
    while(onTargetTwo>22)
        onTargetTwo-=(rand()%5+6);
    if(x==0){
    cout<<"\n\t\tShots for "<<teamOneName<<" : "<<onTargetOne<<endl;
    cout<<"\n\t\tShots for "<<teamTwoName<<" : "<<onTargetTwo<<endl;
    if(i>j)
    {
        cout<<"\n\n\t\t"<<teamName[u]<<" wins!\n";
    }
    else if(j>i)
    {
        cout<<"\n\n\t\t"<<teamName[v]<<" wins!\n";
    }
    else
        cout<<"\n\n\t\t"<<"Draw!\n";
    getch();
    fileCounter++;
    match<<"\n\tMatch number : "<<fileCounter<<endl;
    match<<"\t\t"<<teamOneName<<" "<<countOne<<" - "<<countTwo<<" "<<teamTwoName<<endl<<endl;
    for(z=0;z<91;z++)
    {
        for(y=0;y<i;y++)
        {
            if(goalOne[y][0]==z)
            {
                match<<"\t\t"<<z<<"' "<<finalTeam[u][goalOne[y][1]].name<<" scores for "<<teamOneName<<"!"<<endl;
            }
        }
        for(y=0;y<j;y++)
        {
            if(goalTwo[y][0]==z)
            {
                match<<"\t\t"<<z<<"' "<<finalTeam[v][goalTwo[y][1]].name<<" scores for "<<teamTwoName<<"!"<<endl;
            }
        }
        for(y=0;y<m;y++)
        {
            if(bookOne[y][0]==z)
            {
                match<<"\t\t"<<z<<"' "<<"Yellow Card! "<<finalTeam[u][bookOne[y][1]].name<<"!"<<teamOneName<<"!"<<endl;
            }
        }
        for(y=0;y<n;y++)
        {
            if(bookTwo[y][0]==z)
            {
                match<<"\t\t"<<z<<"' "<<"Yellow Card! "<<finalTeam[v][bookOne[y][1]].name<<"!"<<teamTwoName<<"!"<<endl;
            }
        }
    }

    match<<"\n\t\tShots:\n\t\t"<<teamOneName<<": "<<onTargetOne<<endl;
    match<<"\t\t"<<teamTwoName<<": "<<onTargetTwo<<endl;
    }
    system("cls");
    cout<<"\n\t\tPoints Table\n\n\n";
    for(q=0;q<8;q++)
    {
        cout<<"\t"<<points[q]<<" \t"<<teamName[q]<<endl<<endl;
    }
    getch();
    //cout<<"Here";
    //match<<"\n\n\t\tPosession:\n\t\t"<<teamOneName<<": "<<100*pos1/(pos1+pos2);
    //match<<"\n\t\t"<<teamTwoName<<": "<<100-100*pos1/(pos1+pos2);
}
//***********END OF FUNCTION DEFINITIONS***********************
int main()
{
    unsigned int sval;
    time_t t;
    sval=(unsigned)time(&t);
    srand(sval);
    game();
    getchar();
    return 0;
}
