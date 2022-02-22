#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
 
 //Author: Rishab Das
 

double ** clearMatrix(double** matrix, int r, int c){
    //makes all numbers in the matrix equal to 0
   int i,j;
   for( i= 0; i<r;i++){
       for( j=0;j <c;j++){
          matrix[i][j]=0;
       }
   }
   return matrix;
}
double ** multiply(double ** matrix1, double **matrix2, double** answer, int r, int c, int c1 ){
   //function that multiplies two matrices together and produces result
   int i, j, k;
  
   for( i =0; i< r;i++){
    
       for( j=0;j<c;j++){
           for( k = 0; k<c1;k++){
               answer[i][j] += matrix1[i][k] * matrix2[k][j];
              
           }
 
      
       }
   }
   return answer;
}
double ** inverse(double ** matrix, int r, int c){
    //function that inverses the matrix and return the identity matrix
   int i, p , j;
   double ** identityMatrix= (double**)malloc(r*sizeof(double*));
   for(i=0;i<r;i++){
       identityMatrix[i]= malloc(r * sizeof(double));
   }
   for (i = 0; i < r; i++){
        for (j = 0; j < c; j++){
            if (i == j){
                identityMatrix[i][j]=1;
            }else{
                identityMatrix[i][j]=0;
            }
        }
   }
   //create identity matrix
 
   int count;
   double q;
   for (p = 0; p < r; p++){
       q= matrix[p][p];
       for (count = 0; count < r; count++){
           matrix[p][count] /= q;
           identityMatrix[p][count] /= q;
       }
        for (i = p + 1; i < r; i++){
            q = matrix[i][p];
           for (count = 0; count < r; count++){
               matrix[i][count] -= (q * matrix[p][count]);
               identityMatrix[i][count] -= (q * identityMatrix[p][count]);
           }
        }
   }
    for (p = r - 1; p >= 0; p--){
       for (i = p-1; i >= 0; i--){
           q= matrix[i][p];
           for (count = 0; count < r; count++){
               matrix[i][count] -= (q* matrix[p][count]);
               identityMatrix[i][count] -= (q*identityMatrix[p][count]);
           }
       }
    }
 
   return identityMatrix;
   free(identityMatrix);
}
void printPrice(double ** matrix, int r, int c){
    // print statement that prints final price of 
   int j,i;
   for(i=0;i<r;i++){
       for(j=0;j<c;j++){
           printf("%.0f", matrix[i][j]);
       }
       printf("\n");
   }
}
double **transpose(double **matrix,double **transposed, int r, int c){
   //function to transpose matrix
   int i, j;
   for(i=0;i<c;i++){
       for(j=0; j<r;j++){
           transposed[i][j]= matrix[j][i];
       }
    }
       return transposed;
}
int main(int argc, char **argv){
  
   FILE* file1;
   file1= fopen(argv[1], "r");
   
   //opens first file and check if there is anything there
    int i, j, a, h;
   // a is equal to number attributes and h is equal to number of houses
 
   char *train = malloc(7*sizeof(char) );
   strcpy(train, "");
   fscanf(file1, "%s",train);
   fscanf(file1, "%d", &a);
   fscanf(file1, "%d", &h);
 
   double ** matrixx= malloc( h * sizeof (double * ) );
   double ** vector1 = malloc( h * sizeof (double * ) );
   double ** vector2 = malloc( (a + 1) * sizeof (double*) );
 
   for(i=0; i<h;i++){
       matrixx[i]= malloc((a+1)*sizeof(double));
       vector1[i]= malloc(sizeof(double));
   }
   //alocating space for each element 
   for(i=0;i<a+1;i++){
       vector2[i]=malloc(sizeof(double));
   }
   matrixx=clearMatrix(matrixx,h,a+1 );
   vector1=clearMatrix(vector1,h,1 );
   vector2=clearMatrix(vector2,a+1,1 );
   //clears all matrixs by putting 0 in them
 
   for(i=0; i<h;i++){
       matrixx[i][0]=1;
       for(j=1; j<a+1;j++){
           fscanf(file1, "%lf", &matrixx[i][j]);
       }
       fscanf(file1, "%lf", &vector1[i][0]);
   }
   double ** transposeA = malloc((a+1)*sizeof(double*));
 
   for (i = 0; i < a + 1; i++) {
     transposeA[i] = malloc(h * sizeof(double));
   }
 
   transposeA = clearMatrix(transposeA, a+1, h);
   transposeA = transpose(matrixx, transposeA, h, a+1);
//finding transpose matrix using function
 
   double ** product=  malloc((a + 1)*sizeof(double *));
   for (i = 0; i < a+1; i++) {
     product[i] = malloc((a + 1) * sizeof(double));
   }


   product = clearMatrix(product, a+1,a+1);
   product = multiply(transposeA,matrixx, product, a+1,a+1,h );
 
 //finding the product of matricies after allocating space
 
   double ** inverseA = malloc((a + 1) * sizeof(double *));
   for (i = 0; i < a+1; i++) {
     inverseA[i] = malloc((a + 1) * sizeof(double));
   }
   inverseA = clearMatrix(inverseA, a + 1, a + 1);
   inverseA = inverse(product, a + 1, a + 1);
 
   double ** result1 = malloc((a + 1) * sizeof(double *));
   for (i = 0; i < a + 1; i++) {
       result1[i] = malloc(h * sizeof(double));
   }
   result1 = clearMatrix(result1, a + 1, h);
   result1 = multiply(inverseA, transposeA, result1, a + 1,h, a + 1);
 
   vector2 = multiply(result1, vector1, vector2, a + 1, 1, h);
   //Finish trainer

   // must allocate space for each and every one
   fclose(file1);
 
 
   FILE * file2;
   file2 = fopen(argv[2], "r");
   i = 0, j = 0;
   int a2 = 0, h2;
   char *data = malloc(7 * sizeof(char));
   fscanf(file2, " %s", data);
   fscanf(file2, " %d", &a2);
   fscanf(file2, " %d", &h2);
   if(a != a2){
       printf("error\n");
       return 0;
   }
 
   double ** estimate1 = malloc(h2 * sizeof(double *));
 
   double ** estimate2 = malloc(h2 * sizeof(double *));
 
   for (i = 0; i < h2; i++) {
     estimate1[i] = malloc((a2 + 1) * sizeof(double));
     estimate2[i] = malloc(sizeof(double));
   }
 
   estimate1 = clearMatrix(estimate1, h2, a2 + 1);
 
   estimate2 = clearMatrix(estimate2, h2, 1);
 
   for (i = 0; i < h2; i++) {
     estimate1[i][0] = 1;
     for (j = 1; j < a2 + 1; j++) {
   fscanf(file2, "%lf", &estimate1[i][j]);
     }
   }
 
   estimate2 = multiply(estimate1, vector2, estimate2, h2, 1, a2+1);
 
   printPrice(estimate2, h2, 1);
  fclose(file2);
  free(train);
  free(matrixx);
  free(vector1);
  free(vector2);
  free(transposeA);
  free(product);
  free(inverseA);
 
  free(data);
  free(estimate1);
  free(estimate2);

 
  return 0;
 
}
