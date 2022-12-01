#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/ext/matrix_relational.hpp>

const float THRESHOLD = (float)1.0e-5;
#define HEADER(X) std::cout << std::endl << (X) << std::endl << std::endl;
#define floatEquals(a, b) (std::fabs((a) - (b)) <= THRESHOLD)
#define floatsEquals(a, b) (glm::all(glm::equal(a, b, THRESHOLD)))

using namespace std;
using namespace glm;

mat3 createCoordinateFrame(vec3 view, vec3 up) {

    // Checking the arguments

    if(floatEquals(length(view), 0)) {
        throw "Given View cannot have length 0";
    }

    if(floatEquals(length(up), 0)) {
        throw "Given Up cannot have length 0";
    }

    vec3 v = view / length(view);
    vec3 w = cross(up, v);

    w = w / length(w);

    if(floatEquals(length(w), 0)) {
        // View and Up are collinear
        throw "The given View and Up vector must not be collienear";
    }

    vec3 u = cross(v, w);

    return mat3(v, u, w);
}

mat3 rodriguesFormula(float thetha, vec3 rotationAxis) {

    // Checking the arguments

    float axisLength = length(rotationAxis);

    if(floatEquals(axisLength, 0)) {
        throw "Given rotation axis must have length greater than 0";
    }

    rotationAxis = rotationAxis / axisLength;

    mat3 dualMatrix(0, rotationAxis.z, -rotationAxis.y,
                    -rotationAxis.z, 0, rotationAxis.x,
                    rotationAxis.y, -rotationAxis.x, 0);

    mat3 identify(1.0f);

    return identify + (sin(thetha) * dualMatrix) + (1 - cos(thetha)) * (dualMatrix * dualMatrix);
}


void exercise3(mat3 matrix1, mat3 matrix2) {
    if(floatEquals(determinant(matrix1), 0)) {
        throw "The determinant for the first matrix can't be 0";
    }

    if(floatEquals(determinant(matrix2), 0)) {
        throw "The determinant for the second matrix can't be 0";
    }

    mat3 cal1 = inverse(matrix1 * matrix2);
    cout << "(A,B)^-1 = " << to_string(cal1) << endl;
    mat3 cal2 = inverse(matrix2) * inverse(matrix1);
    cout << "B^-1 * A^-1 = " << to_string(cal2) << endl;
    float f1 = determinant(cal1);
    float f2 = determinant(cal2);

    assert(floatEquals(f1, f2));
}

void printDifferences(mat4 expected, mat4 obtained) {
    cout << "Expected: " << to_string(expected) << ", obtained: " << to_string(obtained) << endl;
}

int main() {

    try {
        // Exercise 1

        HEADER("Exercise 1 tests")

        vec3 view1(2.0f, 0, 0);
        vec3 up1(0.5f, 5.0f, 0);
        mat3 result;

        mat3 correctResult1(1.0f, 0, 0,
                          0, 0, -1.0f,
                          0, 1.0f, 0);

        try {
            cout << "Test 1.1: A view on the x axis, an up on the y axis slightly tilted to x" << endl;

            mat3 result = createCoordinateFrame(view1, up1);

            if(floatsEquals(result, correctResult1)) {
                cout << "Success" << endl;
            }
            else {
                cout << "Failure. Result was not the same as the expected: " << endl;
            }
        } catch(char const* message) {
            cout << "Shouldn't have thrown an error: " << message << endl;
        }

        // Exercise 3

        HEADER("Exercise 3 tests")

        mat3 identity(1.0f);
        cout << "Identity matrix : " << to_string(identity) << endl;

        // Matrix with determinant equal to 0
        mat3 matrix0(1.0f, 2.0f, 1.0f,
                     2.0f, 3.0f, 2.0f,
                     1.0f, 2.0f, 1.0f);
        cout << "Matrix with determinant equal to 0: " << to_string(matrix0) << endl << endl;

        // Random matrix with positive determinant 1
        mat3 matrixp1(4.0f, -1.0f, 1.0f,
                     4.0f, 5.0f, 3.0f,
                     -2.0f, 0.0f, 0.0f);
        cout << "Random matrix with positive determinant 1: " + to_string(matrixp1) << endl << endl;

        // Random matrix with positive determinant 2
        mat3x3 matrixp2(4.0f, -1.0f, 1.0f,
                       4.0f, 5.0f, 3.0f,
                       -3.0f, 2.0f, 5.0f);
        cout << "Random matrix with positive determinant 2: " + to_string(matrixp2) << endl << endl;

        // Random matrix with negative determinant 1
        mat3 matrixn1(1.0f, 2.0f, 3.0f,
                     2.0f, 3.0f, 2.0f,
                     3.0f, 2.0f, 1.0f);
        cout << "Random matrix with negative determinant 1: " + to_string(matrixn1) << endl << endl;

        // Random matrix with negative determinant 2
        mat3 matrixn2(1.0f, 2.0f, 3.0f,
                     3.0f, 3.0f, -2.0f,
                     3.0f, 2.0f, 1.0f);
        cout << "Random matrix with negative determinant 2: " + to_string(matrixn2) << endl << endl;

        try {
            cout << "Test 1: 2 identity matrices" << endl;
            exercise3(identity, identity);

            cout << "Success" << endl;
        } catch(char const* message) {
            cout << "Shouldn't have thrown an error: " << message << endl;
        }

        try {
            cout << "Test 2: 1 matrix with determinant 0 and 1 matrix with determinant equal to 0" << endl;
            exercise3(matrix0, identity);

            cout << "Should've thrown an error" << endl;
        } catch(char const* message) {
            cout << "Success" << endl;
        }

        try {
            cout << "Test 3: 1 identity matrix and 1 matrix with determinant equal to 0" << endl;
            exercise3(identity, matrix0);

            cout << "Should've thrown an error" << endl;
        } catch(char const* message) {
            cout << "Success" << endl;
        }

        try {
            cout << "Test 4: 1 matrix with positive determinant and 1 identity matrix" << endl;
            exercise3(matrixp1, identity);

            cout << "Success" << endl;
        } catch(char const* message) {
            cout << "Shouldn't have thrown an error: " << message << endl;
        }

        try {
            cout << "Test 5: 1 negative matrix with positive determinant and 1 identity matrix" << endl;
            exercise3(matrixn1, identity);

            cout << "Success" << endl;
        } catch(char const* message) {
            cout << "Shouldn't have thrown an error: " << message << endl;
        }

        try {
            cout << "Test 6: 1 matrix with positive determinant and 1 matrix with negative determinant" << endl;
            exercise3(matrixp1, matrixn1);

            cout << "Success" << endl;
        } catch(char const* message) {
            cout << "Shouldn't have thrown an error: " << message << endl;
        }

        try {
            cout << "Test 7" << endl;
            exercise3(matrixp1, matrixn2);

            cout << "Success" << endl;
        } catch(char const* message) {
            cout << "Shouldn't have thrown an error: " << message << endl;
        }

        try {
            cout << "Test 8" << endl;
            exercise3(matrixp1, matrixp2);

            cout << "Success" << endl;
        } catch(char const* message) {
            cout << "Shouldn't have thrown an error: " << message << endl;
        }

        try {
            cout << "Test 9" << endl;
            exercise3(matrixp2, matrixn1);

            cout << "Success" << endl;
        } catch(char const* message) {
            cout << "Shouldn't have thrown an error: " << message << endl;
        }

        try {
            cout << "Test 10" << endl;
            exercise3(matrixp2, matrixn2);

            cout << "Success" << endl;
        } catch(char const* message) {
            cout << "Shouldn't have thrown an error: " << message << endl;
        }

    } catch (char const* message) {
        cout << "Unexpected error: " << message << endl;
    }
}


