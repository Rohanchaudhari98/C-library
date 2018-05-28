#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"intal.h"

typedef struct number{
	int len;
	char *num;
}number;

void* intal_create(const char* str){
	int k;
	int j = 0;
	int n = strlen(str);
	if(str == NULL)
		return NULL;
	number *intal = (number*)malloc(sizeof(number));
	while(j<n-1 && str[j]=='0')		
		j++; 
	k = j;
	intal->num = (char*)malloc(n-j+1);
	for(int i=n-k-1;i>=0;i--,j++){
		intal->num[i] = str[j]-'0';
		if(intal->num[i]>9 || intal->num[i]<0){
			free(intal->num);
			free(intal);
			return NULL;
		}
	}
	intal->num[n-k] = '\0';
	intal->len = n-k;
	return intal;		
}



char* intal2str(void* var){
	number *intal = (number*)var;
	char *num;
	int n = intal->len;
	num = (char*)malloc(n+1);
	for(int i=0,j=n-1;i<n,j>=0;i++,j--)
		num[j] = intal->num[i]+'0';
	num[n] = '\0';
	return num;
}

void* intal_increment(void* var){
	number *intal = (number*)var;
	number *res = (number*)malloc(sizeof(number));
	res->num = (char*)malloc(intal->len+1);
	int i,n,carry;
	if(intal == NULL)
		return NULL;
	n = intal->len;
	i = 0;
	do{
		res->num[i] = intal->num[i]+1;
		carry = res->num[i] > 9? 1: 0;
		res->num[i] = carry==0? res->num[i]: 0;
		i++;
	}while(i<n && carry!=0);
	while(i<n){
		res->num[i] = intal->num[i];
		i++;
	}
	if(carry==1){
		res->num[i] = 1;
		n++;
	}
	free(intal->num);
	free(intal);
	res->len = n;
	return res;
}

void* intal_decrement(void* var){
	number *intal = (number*)var;
	number *res = (number*)malloc(sizeof(number));
	int i,n,borrow;
	if(intal == NULL)
		return NULL;
	n = intal->len;
	i = 0;
	if(n==1 && intal->num[0]==0){
		res->num = (char*)malloc(1);
		res->len = 1;
		res->num[0] = 0;
		free(intal->num);
		free(intal);
		return res;
	}
	res->num = (char*)malloc(n+1);
	while(intal->num[i]==0 && i<n){
		res->num[i]=9;
		i++;
	}
	res->num[i]=intal->num[i]-1;
	i++;
	while(i<n){
		res->num[i] = intal->num[i];
		i++;
	}
	if(res->num[i-1]==0){
		n--;
	}
	res->len = n;
	free(intal->num);
	free(intal);
	return res;
}

void* intal_add(void* var1, void* var2){
	number *intal1 = (number*)var1;
	number *intal2 = (number*)var2;
	int i,n,carry,len;
	number *sum = (number*)malloc(sizeof(number));
	len = intal1->len > intal2->len? intal1->len : intal2->len;	
	sum->num = (char*)malloc(len+2);
	if(intal1==NULL || intal2==NULL)
		return NULL;
	n = intal1->len < intal2->len? intal1->len : intal2->len;
	carry = 0;						
	i = 0;
	while(i<n){
		sum->num[i] = intal1->num[i] + intal2->num[i] + carry;
		carry = sum->num[i] > 9? sum->num[i]/10 : 0;
		sum->num[i] = sum->num[i]%10;
		i++;
	}
	while(i < intal1->len){
		sum->num[i] = intal1->num[i] + carry;
		carry = sum->num[i] > 9? sum->num[i]/10 : 0;
		sum->num[i] = sum->num[i]%10;
		i++;
	}
	while(i < intal2->len){
		sum->num[i] = intal2->num[i] + carry;
		carry = sum->num[i] > 9? sum->num[i]/10 : 0;
		sum->num[i] = sum->num[i]%10;
		i++;
	}
	if(carry>0){
		sum->num[i] = carry;
		i++;
	}
	sum->len = i;
	return sum;
}

void* intal_diff(void* var1, void* var2){
	number *intal1 = (number*)var1;
	number *intal2 = (number*)var2;
	int cmp,i,nb,ns,sub,borrow;					
	number *big,*small,*diff;
	if(intal1 == NULL || intal2 == NULL)
		return NULL;
	diff = (number*)malloc(sizeof(number));
	cmp = intal_compare(intal1,intal2);
	if(cmp==0){							
		diff->len = 1;
		diff->num = (char*)malloc(1);
		diff->num[0] = 0;
		return diff;
	}
	else if(cmp==1){
		big = intal1;
		small = intal2;
	}
	else{
		big = intal2;
		small = intal1;
	}
	nb = big->len;
	ns = small->len;
	i = 0;
	diff->num = (char*)malloc(nb+1);
	diff->num[0] = 0;
	while(i < ns){
		sub = big->num[i] - small->num[i];	
		if(sub+diff->num[i] < 0){
			diff->num[i] += sub+10;
			diff->num[i+1] = -1;
		}
		else{
			diff->num[i] += sub;
			diff->num[i+1] = 0;
		}
		i++;
	}
	while(i<nb){
		diff->num[i] += big->num[i];
		if(diff->num[i] < 0){
			diff->num[i] = 9;
			diff->num[i+1] = -1;
		}
		else
			diff->num[i+1] = 0;
		i++;
	}
	while(i>0 && diff->num[i]==0){
		i--;
	}
	diff->len = i+1;
	return diff;
}

int intal_compare(void* var1, void* var2){
	number *intal1 = (number*)var1;
	number *intal2 = (number*)var2;
	int i,cmp;
	if(intal1->len > intal2->len)
		return 1;
	else if(intal1->len < intal2->len)
		return -1;
	for(i = intal1->len-1; i>=0; i--){
		if(intal1->num[i] > intal2->num[i])
			return 1;
		else if(intal1->num[i] < intal2->num[i])
			return -1;
	}
	return 0;
}

number** split(number* var1, int m){
	int i,n;
	number *intal1 = (number*)var1;
	number *a1 = (number*)malloc(sizeof(number));
	number *a2 = (number*)malloc(sizeof(number));
	number **arr = (number**)malloc(sizeof(number*)*2);
	a1->len = a2->len = m;
	a1->num = intal1->num + m;
	a2->num = intal1->num;
	arr[0] = a2;
	arr[1] = a1;
	return arr;
}

void* intal_divide(void* var1, void* var2){
	int cmp,n1,n2,len,i,j;
	number *substr = (number*)malloc(sizeof(number));
	number *remainder;
	number *temp;	
	number *buffer;
	number *intal1 = (number*)var1;
	number *intal2 = (number*)var2;
	number *res = (number*)malloc(sizeof(number));	 
	if(intal1==NULL || intal2==NULL)
		return NULL;
	if(intal2->len == 1 && intal2->num[0] == 0)
		return NULL;
	cmp = intal_compare((void*)intal1,(void*)intal2);
	if(cmp==0){
		res->num = (char*)malloc(2);
		res->num[0] = 1;
		res->len = 1;
		return res;
	}
	else if (cmp==-1){
		res->num = (char*)malloc(2);
		res->num[0] = 0;
		res->len = 1;
		return res;
	}
	len = intal1->len;
	n1 = len;
	n2 = intal2->len;
	buffer = (number*)malloc(sizeof(number));
	buffer->num = (char*)malloc(intal1->len+1);
	res->num = (char*)malloc(intal1->len+1);	
	res->len = intal1->len;
	for(i=0;i<n1;i++)
		buffer->num[i] = intal1->num[i];
	buffer->len = n1;
	substr->num = buffer->num + n1 - 1;
	substr->len = 1;
	i = n1-1;
	while(i>=0){
		for(;i>=0 && substr->len<n2;i--){
			substr->num--;
			substr->len++;
			res->num[i] = 0; 
		}
		while(intal_compare((void*)substr, (void*)intal2)==-1 && i>=0){
			substr->num--;
			substr->len++;
			res->num[i] = 0;
			i--;
		}
		if(i<0){
			break;
		}			
		remainder = intal_diff((void*)substr, (void*)intal2);
		res->num[i] = 1;
		while(intal_compare((void*)remainder, (void*)intal2)>-1){
			temp = intal_diff((void*)remainder, (void*)intal2);
			free(remainder->num);
			free(remainder);
			remainder = temp;
			res->num[i]++;
		}
		for(j=0;j<remainder->len;j++){
			substr->num[j] = remainder->num[j];
		}
		substr->num--;	
		substr->len = j+1;
		free(remainder->num);
		free(remainder);
		i--;
	}	
	free(buffer->num);
	free(buffer);
	free(substr);
	i = res->len-1;
	while(i>0 && res->num[i]==0) 
		i--;
	res->len = i+1;
	return res;
}

void* intal_pow(void* var1, void* var2){

	number *intal1 = (number*)var1;
	number *intal2 = (number*)var2;
	number *res=NULL;
	number *res2=NULL;
	number *res3=NULL;
	number *two=NULL;
	number *half=NULL;
	if(intal2->len==1 && intal2->num[0]==0){
		res = (number*)malloc(sizeof(number));
		res->num = (char*)malloc(2);
		res->num[0] = 1;
		res->len = 1;
		return res;
	}

	two = (number*)malloc(sizeof(number));	 
	two->num = (char*)malloc(2);
	two->num[0] = 2;
	two->len = 1;

	half = intal_divide(intal2, two);
	
	res = intal_pow((void*)intal1, (void*)half);
	
	res2 = intal_multiply((void*)res, (void*)res);
	if(intal2->num[0]%2==1){
		res3 = intal_multiply(res2, intal1);
	}

	intal_destroy(res);
	intal_destroy(two);
	intal_destroy(half);
	if(res3!=NULL){
		res = res3;
		intal_destroy(res2);
	}
	else{
		res = res2;
	}
	return res;

}

number* karatsuba(number* var1, number* var2){
	int prod,k,m,i,j,n;	
	int flag = 0;
	char old1,old2;

	number *a1,*a2,*b1,*b2;
	number **arr1,**arr2;
	
	number *e1,*e2;	
	
	number *res,*p1,*p2,*p3,*sumA,*sumB;
	number *diff31, *diff312;	
	
	number *add2, *add1;	

	number *intal1 = (number*)var1;
	number *intal2 = (number*)var2;
	i = intal1->len;

	while(i < intal2->len){
		intal1->num[i] = 0;
		i++;
	}
	intal1->len = i;
	i = intal2->len;
	while(i < intal1->len){
		intal2->num[i] = 0;
		i++;
	}
	intal2->len = i;
	n = intal1->len; 
	res = (number*)malloc(sizeof(number));
	res->num = (char*)malloc(2*n+1);
	if(n==1){
		prod = intal1->num[0]*intal2->num[0];
		if(prod > 9){
			res->num[0] = prod%10;
			res->num[1] = prod/10;
			res->len = 2;
			return res;
		}
		res->num[0] = prod;
		res->len = 1;
		return res;
	}
	if(n%2 == 1){
		old1 = intal1->num[n];
		intal1->num[n] = 0;
		intal1->len++;
		old2 = intal2->num[n];
		intal2->num[n] = 0;
		intal2->len++;
		n++;
		flag = 1;
	}
	m = n/2;
	arr1 = split(intal1, m);
	arr2 = split(intal2, m);
	
	a1 = arr1[1];
	a2 = arr1[0];	 
	b1 = arr2[1];
	b2 = arr2[0];
	p1 = karatsuba(a1,b1);
	p2 = karatsuba(a2,b2);
	sumA = (number*)intal_add((void*)a1,(void*)a2);
	sumB = (number*)intal_add((void*)b1,(void*)b2);
	p3 = karatsuba(sumA,sumB);

	free(a1);	
	free(a2);
	free(b1);
	free(b2);
	free(arr1);
	free(arr2);
	intal_destroy(sumA);
	intal_destroy(sumB);
	
	remove_zeroes(p1);
	remove_zeroes(p2);
	remove_zeroes(p3);

	diff31 = (number*)intal_diff((void*)p3,(void*)p1);
	
	diff312 = (number*)intal_diff((void*)diff31,(void*)p2);
	e2 = (number*)malloc(sizeof(number));
	e2->len = p2->len > m? p2->len - m : 0;
	e2->num = p2->num + m ;
	add2 = (number*)intal_add((void*)e2,(void*)diff312);
	
	e1 = (number*)malloc(sizeof(number));
	e1->len = add2->len > m? add2->len - m: 0;
	e1->num = add2->num + m ;
	add1 = (number*)intal_add((void*)e1,(void*) p1);
	for(i=0;i<m && i< p2->len;i++)
		res->num[i] = p2->num[i]; 
	for(;i<m;i++)
		res->num[i] = 0;
	for(j=0;i<n && j<add2->len;i++,j++)
		res->num[i] = add2->num[j];
	for(;i<n;i++)
		res->num[i] = 0;
	for(j=0;j < add1->len;i++,j++)
		res->num[i] = add1->num[j];
	i--;
	while(i>0 && res->num[i]==0){
		i--;
	}
	res->len = i+1;

	intal_destroy(diff31);
	intal_destroy(diff312);
		intal_destroy(add1);
	intal_destroy(add2);
	intal_destroy(p1);
	intal_destroy(p2);
	intal_destroy(p3);
	free(e1);
	free(e2);
	
	if(flag==1){
		n--;
		intal1->num[n] = old1;
		intal2->num[n] = old2;
		intal1->len--;
		intal2->len--;
	}
	return res;
}


void* intal_multiply(void* var1, void* var2){
	int i;
	number *arg1 = (number*)var1;
	number *arg2 = (number*)var2;
	number intal1,intal2;
	number *res;

	int len = arg1->len > arg2->len? arg1->len : arg2->len;
	len = len%2 == 0? len: len + 1;
	intal1.num = (char*)malloc(len+2);
	intal2.num = (char*)malloc(len+2);
	for(i=0;i<arg1->len;i++)		
		intal1.num[i] = arg1->num[i];
	for(;i<len;i++)		
		intal1.num[i] = 0;
	intal1.len = len;
	for(i=0;i<arg2->len;i++)
		intal2.num[i] = arg2->num[i];
	for(;i<len;i++)
		intal2.num[i] = 0;
	intal2.len = len;
	res = karatsuba(&intal1, &intal2);
	free(intal1.num);
	free(intal2.num);
	return res;
}


void intal_destroy(void* var){
	number *intal = (number*)var;
	if(intal == NULL)
		return;
	free(intal->num);
	free(var);
	var = NULL;
}

void remove_zeroes(number* intal){
	int i = intal->len-1;
	while(i>0 && intal->num[i]==0)
		i--;
	intal->len = i+1;
}