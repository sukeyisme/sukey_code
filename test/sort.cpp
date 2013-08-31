#include <stdio.h>
#include <algorithm>
#include <time.h>
#include <stack>
using namespace std;

//void ShellSort(int item[],long lStart,long lEnd)
//{
//	int i,h;
//	for(h=1;h<=(lEnd-lStart)/9;h=3*h+1);
//	for(;h>0;h/=3)
//	for(i=lStart+h;i<=lEnd;++i)
//	{
//		int j = i;int v = item[i];
//		while(j>=lStart+h&&(v<item[j-h]))
//		{
//			item[j] = item[j-h];
//			j -= h;
//		}
//		item[j] = v;
//	}
//}

long* ShellSort(int item[],int n)
{
	long* count= new long(2);
	memset(count,0,sizeof(long)*2);
	int i,j;
	for(int d=n/2;d>=1;d=d/2)
	{
		for(i=d;i<n;++i)
		{
			int t = item[i];
			for(j=i-d;j>=0&&t<item[j];j=j-d,count[0]++)
			{
				item[j+d] = item[j];
				count[1]++;
			}
			item[j+d] = t;
			count[1]++;
		}
	}
	return count;
}

void Swap(int &a,int &b)
{
	int temp = a;
	a = b;
	b = temp;
}


long* SelectSort(int item[],long n)
{
	long* count= new long(2);
	memset(count,0,sizeof(long)*2);
	int small(0);
	for(int i=0; i<n-1;i++)
	{
		small = i;
		for(int j=i+1;j<n;++j)
		{
			count[0]++;
			if(item[j]<item[small])
				small=j;
		}
		if(small!=i)
		{
			Swap(item[i],item[small]);
			count[1]++;
		}
	}
	return count;
}

long* InsertSort(int item[],long n)
{
	long* count= new long(2);
	memset(count,0,sizeof(long)*2);
	int temp(0);
	for(int i=1;i<n;++i)
	{
		int j=i;
		temp = item[i];
		count[0]++;
		while(j>0&&temp<item[j-1])
		{
			item[j]=item[j-1];
			j--;
			count[1]++;
		}
		item[j] = temp;
	}
	return count;
}

long* BubbleSort(int item[],long n)
{
	long* count= new long(2);
	memset(count,0,sizeof(long)*2);
	int i,j,last;
	i = n-1;
	while(i>0)
	{
		last = 0;
		for(j=0; j<i;++j)
		if(item[j+1]<item[j]&&count[0]++)
		{
			Swap(item[j],item[j+1]);
			count[1]++;
			last = j;
		}
		i=last;	
	}
	return count;
}

long* QuickSort(int item[],int left,int right)
{
	long* count= new long(2);
	memset(count,0,sizeof(long)*2);
	int i,j;
	if(left<right)
	{
		i = left;j=right+1;
		do
		{
			do i++;while (++count[0]&&item[i]<item[left]);
			do j--;while (++count[0]&&item[j]>item[left]);
			if(i<j)
			{
				count[1]++;
				Swap(item[i],item[j]);
			}
		}while (i<j);
		Swap(item[left],item[j]);
		count[1]++;
		QuickSort(item,left,j-1);
		QuickSort(item,j+1,right);
	}
	return count;
}

int QSort(int item[],int left,int right,long count[])
{
	int i,j;
	if(left<right)
	{
		//count[0]++;
		i = left;j=right+1;
		do
		{
			do i++;while (item[i]<item[left]&&count[0]++);
			do j--;while (item[j]>item[left]&&count[0]++);
			if(i<j) 
			{
				count[1]++;
				Swap(item[i],item[j]);
			}

		}while (i<j);
		Swap(item[left],item[j]);
		count[1]++;
	}
	return j;
}

long* QuickSort2(int item[],int left,int right)
{
	long* count= new long(2);
	memset(count,0,sizeof(long)*2);
	 stack<int> st;
	 if(left<right)
	 {
        int mid=QSort(item,left,right,count);
        if(left<mid-1)
		{
            st.push(left);
            st.push(mid-1);
        }
        if(mid+1<right)
		{
            st.push(mid+1);
            st.push(right);
        }
		while(!st.empty())
		{
			int q=st.top();
            st.pop();
            int p=st.top();
            st.pop();
            mid=QSort(item,p,q,count);
            if(p<mid-1){
				count[0]++;
                st.push(p);
                st.push(mid-1);
            }
            if(mid+1<q){
				count[0]++;
                st.push(mid+1);
                st.push(q);
            }       
		}
	 }
	 return &(count[0]);
}


int main()
{
	long i(0);
	const int kNumTest = 30000;
	const int kTestRange = 30000;
	int itemrand[kNumTest] = {0};
	int itemup[kNumTest] = {0};
	int itemdown[kNumTest]= {0};
	int item[kNumTest] = {0};
	long* count(NULL);
	clock_t start(0),end(0);
	double a(0);

	while(i<kNumTest)
	{
		itemrand[i] = rand()%kTestRange;
		itemup[i] = i;
		itemdown[i] = kNumTest-i-1;
		i++;
	}

	printf("ÅÅÐò·½·¨\tÐèÒªÊ±¼ä\t±È½Ï´ÎÊý\tÒÆ¶¯´ÎÊý\n");
	printf("--------------Ëæ»úÊýÅÅÐò--------------\n");
	memcpy(item,itemrand,sizeof(itemrand));
	start = clock();
	count = SelectSort(item,kNumTest);//¼òµ¥Ñ¡ÔñÅÅÐò
	end = clock();
	a = (end-start)/(double)CLK_TCK;
	printf("¼òµ¥Ñ¡ÔñÅÅÐò:\t%f\t%d\t%d\n",a,count[0],count[1]);

	memcpy(item,itemrand,sizeof(itemrand));
	start = clock();
	count = InsertSort(item,kNumTest);//Ö±½Ó²åÈëÅÅÐò
	end = clock();
	a = (end-start)/(double)CLK_TCK;
	printf("Ö±½Ó²åÈëÅÅÐò:\t%f\t%d\t%d\n",a,count[0],count[1]);

	memcpy(item,itemrand,sizeof(itemrand));
	start = clock();
	count = ShellSort(item,kNumTest);//Ï£¶ûÅÅÐò
	end = clock();
	a = (end-start)/(double)CLK_TCK;
	printf("Ï£¶ûÅÅÐò:\t%f\t%d\t%d\n",a,count[0],count[1]);

	memcpy(item,itemrand,sizeof(itemrand));
	start = clock();
	count = BubbleSort(item,kNumTest);//Ã°ÅÝÅÅÐò
	end = clock();
	a = (end-start)/(double)CLK_TCK;
	printf("Ã°ÅÝÅÅÐò:\t%f\t%d\t%d\n",a,count[0],count[1]);

	memcpy(item,itemrand,sizeof(itemrand));
	start = clock();
	count = QuickSort(item,0,kNumTest-1);//¿ìËÙÅÅÐò
	end = clock();
	a = (end-start)/(double)CLK_TCK;
	printf("¿ìËÙÅÅÐò:\t%f\t%d\t%d\n",a,count[0],count[1]);

	printf("--------------ÉýÐòÊýÅÅÐò--------------\n");
	memcpy(item,itemup,sizeof(itemrand));
	start = clock();
	count = SelectSort(item,kNumTest);//¼òµ¥Ñ¡ÔñÅÅÐò
	end = clock();
	a = (end-start)/(double)CLK_TCK;
	printf("¼òµ¥Ñ¡ÔñÅÅÐò:\t%f\t%d\t%d\n",a,count[0],count[1]);

	memcpy(item,itemup,sizeof(itemrand));
	start = clock();
	count = InsertSort(item,kNumTest);//Ö±½Ó²åÈëÅÅÐò
	end = clock();
	a = (end-start)/(double)CLK_TCK;
	printf("Ö±½Ó²åÈëÅÅÐò:\t%f\t%d\t%d\n",a,count[0],count[1]);

	memcpy(item,itemup,sizeof(itemrand));
	start = clock();
	count = ShellSort(item,kNumTest);//Ï£¶ûÅÅÐò
	end = clock();
	a = (end-start)/(double)CLK_TCK;
	printf("Ï£¶ûÅÅÐò:\t%f\t%d\t%d\n",a,count[0],count[1]);

	memcpy(item,itemup,sizeof(itemrand));
	start = clock();
	count = BubbleSort(item,kNumTest);//Ã°ÅÝÅÅÐò
	end = clock();
	a = (end-start)/(double)CLK_TCK;
	printf("Ã°ÅÝÅÅÐò:\t%f\t%d\t%d\n",a,count[0],count[1]);

	//memcpy(item,itemup,sizeof(itemrand));
	//start = clock();
	//QuickSort(item,0,kNumTest-1);//¿ìËÙÅÅÐò
	//end = clock();
	//a = (end-start)/(double)CLK_TCK;
	//printf("¿ìËÙÅÅÐò:\t%f\t%d\t%d\n",a,count[0],count[1]);

	memcpy(item,itemdown,sizeof(item));
	start = clock();
	count = QuickSort2(item,0,kNumTest-1);//¿ìËÙÅÅÐò
	end = clock();
	a = (end-start)/(double)CLK_TCK;
	printf("¿ìËÙÅÅÐò(·ÇµÝ¹é):\t%f\t%d\t%d\n",a,count[0],count[1]);

	printf("--------------½µÐòÊýÅÅÐò--------------\n");
	memcpy(item,itemdown,sizeof(item));
	start = clock();
	count = SelectSort(item,kNumTest);//¼òµ¥Ñ¡ÔñÅÅÐò
	end = clock();
	a = (end-start)/(double)CLK_TCK;
	printf("¼òµ¥Ñ¡ÔñÅÅÐò:\t%f\t%d\t%d\n",a,count[0],count[1]);

	memcpy(item,itemdown,sizeof(item));
	start = clock();
	count = InsertSort(item,kNumTest);//Ö±½Ó²åÈëÅÅÐò
	end = clock();
	a = (end-start)/(double)CLK_TCK;
	printf("Ö±½Ó²åÈëÅÅÐò:\t%f\t%d\t%d\n",a,count[0],count[1]);

	memcpy(item,itemdown,sizeof(item));
	start = clock();
	count = ShellSort(item,kNumTest);//Ï£¶ûÅÅÐò
	end = clock();
	a = (end-start)/(double)CLK_TCK;
	printf("Ï£¶ûÅÅÐò:\t%f\t%d\t%d\n",a,count[0],count[1]);

	memcpy(item,itemdown,sizeof(item));
	start = clock();
	count = BubbleSort(item,kNumTest);//Ã°ÅÝÅÅÐò
	end = clock();
	a = (end-start)/(double)CLK_TCK;
	printf("Ã°ÅÝÅÅÐò:\t%f\t%d\t%d\n",a,count[0],count[1]);

	//memcpy(item,itemdown,sizeof(item));
	//start = clock();
	//QuickSort(item,0,kNumTest-1);//¿ìËÙÅÅÐò
	//end = clock();
	//a = (end-start)/(double)CLK_TCK;
	//printf("¿ìËÙÅÅÐò:\t%f\t%d\t%d\n",a,count[0],count[1]);

	memcpy(item,itemdown,sizeof(item));
	start = clock();
	count = QuickSort2(item,0,kNumTest-1);//¿ìËÙÅÅÐò
	end = clock();
	a = (end-start)/(double)CLK_TCK;
	printf("¿ìËÙÅÅÐò(·ÇµÝ¹é):\t%f\t%d\t%d\n",a,count[0],count[1]);
	
	/*for(i=0;i<kNumTest;++i)
		printf("%d ",item[i]);*/

	return 0;
}

