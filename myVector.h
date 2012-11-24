#ifndef VECTOR
#define VECTOR

#include<iostream>
#include<cmath>
#include<stdlib.h>

#define PI 3.14159265

using namespace std;

// myVector.h  starts
class myVector{
  public:
    float x,y,z;
    myVector(); // Default Constructor
    myVector(float x1,float y1,float z1); //Custom Constructor

    // Special Vector Functions
    float mod(); // returns the magnitude of the vector as in euclidean space 
    float dot(myVector op1); // Returns the dot product of the 'this' vector and the operand vector  ::: 'this' . op1
    
    myVector crossProduct(myVector op1); // Returns the cross product of 'this' vector and op1 vector ::: 'this' x op1
    
    myVector unitVector(); // returns a unit vector of the given vector

    myVector rotateVector(myVector axis, float theta); // Rotates a vector about an 'axis' vector by an angle 'theta' in radians
    
    myVector rotateVectorByDegrees(myVector axis, float theta); // Rotates a vector about an 'axis' vector by an angle 'theta' in degrees
    
    void assign(float x1,float y1, float z1); // assigns an array of floats to a vector
    
    myVector normalise(); // normalises the vector !! makes the vector a unit vector along that direction    lso returns the normalised vector.
    // Operator Overloads
    myVector& operator=(const myVector& op1 ); // Overloading the '=' assignment operator
    friend myVector operator+(const myVector op1, const myVector op2); // Overloading '+' operator for both vectors;
    friend myVector operator-(const myVector op1, const myVector op2); // Overloading '-' operator for both vectors;
   

    friend myVector operator*(const myVector op1, const float op2); // Overloading '*' operator for scalar multiplication
    friend myVector operator/(const myVector op1, const float op2); // Overloading '/' operator for scalar multiplication

    friend ostream& operator<<(ostream& cout, myVector obj); // Overloading << operator for Cout -ing the Vector
};
// myVector.h ends

// -----------------------------------------------------------------------------------------------------------------

#endif //VECTOR
