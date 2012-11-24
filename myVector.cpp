#include "myVector.h"

//myVector.cpp starts

myVector::myVector(){ // Default Constructor
x=y=z=0;
}

myVector::myVector(float x1, float y1, float z1){ // Custom Constructor
  x=x1;
  y=y1;
  z=z1;
}

// Member Functions

float myVector::mod(){ // Magnitude of the vector as in euclidean space
  return sqrt(x*x + y*y + z*z);
}
float myVector::dot(myVector op1){
  return x*op1.x + y*op1.y + z*op1.z;
}

myVector myVector::crossProduct(myVector op1) {

myVector temp;
  temp.x = y*op1.z - z*op1.y ;
  temp.y = z*op1.x - x*op1.z ;
  temp.z = x*op1.y - y*op1.x ;
return temp;

}

myVector myVector::unitVector(){
  return *this / mod();
}

void myVector::assign(float x1,float y1,float z1){
  // Might be a bit buggy here !! The list needs to be checked for length match before processing
  x=x1;
  y=y1;
  z=z1;
}

myVector myVector::normalise(){
  *this = unitVector();

  return *this;
}

myVector myVector::rotateVector(myVector axis, float theta) { // Rotates a vector about an 'axis' by an angle 'theta' in radians

// Theory  obtained from http://inside.mines.edu/~gmurray/ArbitraryAxisRotation/ , Section : Matrix for Rotations about origin 

// *this = <x,y,z>
// axis = <u,v,w>
//theta = angle in radians

// *this dotProduct axis = ux + vy + wz

float dotProduct = dot(axis);

//cout<<"Dot Product : "<<dotProduct<<endl;

float theta1=theta;
float axisMod = axis.mod();

myVector Variables(1-cos(theta1), cos(theta1), sin(theta1));

Variables.x = Variables.x * dotProduct;
Variables.y = Variables.y * pow(axisMod,2);
Variables.z = Variables.z * axisMod;

//cout<<"Variables : "<<Variables;

myVector row1(axis.x , x , -axis.z*y + axis.y*z);
myVector row2(axis.y , y , -axis.x*z + axis.z*x);
myVector row3(axis.z , z , -axis.y*x + axis.x*y);

myVector result(row1.dot(Variables), row2.dot(Variables), row3.dot(Variables));

//cout<<"Unnormalised Result : "<<result;

result = result/pow(axis.mod(),2);

return result;
}

myVector myVector::rotateVectorByDegrees(myVector axis, float theta) { // Rotates a vector about an 'axis' by an angle 'theta' in degrees

// Theory  obtained from http://inside.mines.edu/~gmurray/ArbitraryAxisRotation/ , Section : Matrix for Rotations about origin 

// *this = <x,y,z>
// axis = <u,v,w>
//theta = angle in radians

// *this dotProduct axis = ux + vy + wz

float dotProduct = dot(axis);

//cout<<"Dot Product : "<<dotProduct<<endl;

float theta1=(theta*PI)/180;
float axisMod = axis.mod();

myVector Variables(1-cos(theta1), cos(theta1), sin(theta1));

Variables.x = Variables.x * dotProduct;
Variables.y = Variables.y * pow(axisMod,2);
Variables.z = Variables.z * axisMod;

//cout<<"Variables : "<<Variables;

myVector row1(axis.x , x , -axis.z*y + axis.y*z);
myVector row2(axis.y , y , -axis.x*z + axis.z*x);
myVector row3(axis.z , z , -axis.y*x + axis.x*y);

myVector result(row1.dot(Variables), row2.dot(Variables), row3.dot(Variables));

//cout<<"Unnormalised Result : "<<result;

result = result/pow(axis.mod(),2);

return result;
}

// Operator Overloads
myVector operator+(const myVector op1, const myVector op2){ // Overloading '+' operator
  myVector temp;

  temp.x = op1.x + op2.x;
  temp.y = op1.y + op2.y;
  temp.z = op1.z + op2.z;

  return temp;
}

myVector operator-(const myVector op1, const myVector op2){ // Overloading '-' operator
  myVector temp;

  temp.x = op1.x - op2.x;
  temp.y = op1.y - op2.y;
  temp.z = op1.z - op2.z;

  return temp;
}

myVector operator*(const myVector op1, const float op2){ // Overloading '*' operato for scalar multiplication
  myVector temp;

  temp.x = op1.x * op2;
  temp.y = op1.y * op2;
  temp.z = op1.z * op2;

  return temp;
}

myVector operator/(const myVector op1, const float op2){ // Overloading '/' operation for scalar division
  if(op2==0) // Error check to avoid division by 0 !! Leave as it is !!
  {
    return op1;
  }
  
  myVector temp;

  temp.x = op1.x / op2;
  temp.y = op1.y / op2;
  temp.z = op1.z / op2;

  return temp;
}

myVector& myVector::operator=(const myVector& op1) { // Overloading the '=' assignment operator
  x = op1.x;
  y = op1.y;
  z = op1.z;
  return *this;
}

ostream& operator<<(ostream& cout, myVector obj) { // Overloading << operator for cout output
    cout <<" ( "<<obj.x<<" , "<<obj.y<<" , "<<obj.z<<" )"<<endl;
    return cout;
  }
// myVector.cpp ends

//----------------------------------------------------------------------------------------------

// testing starts
/**
main(){

myVector a(1,0,0);
myVector b(0,0,1);

cout<<a.rotateVectorByDegrees(b,90);

myVector c;
cout<<c<<endl;
c.assign(0.0f,0.1f, 1.0f);
cout<<c;
}
**/
