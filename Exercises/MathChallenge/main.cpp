#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

const float THRESHOLD = (float)1.0e-5;
#define HEADER(X) std::cout << std::endl << (X) << std::endl << std::endl;
#define floatEquals(a, b) (std::fabs(a - b) <= THRESHOLD)

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

int main() {

    try {
        // Exercise 1

        HEADER("Exercise 1 tests")

        vec3 view(2.0f, 0, 0);
        vec3 up(0.5f, 5.0f, 0);
        mat3 frame = createCoordinateFrame(view, up);

        cout << "First coordinate frame: " << to_string(frame) << endl;

        // Exercise 3

        HEADER("Exercise 3 tests")

        mat3 matrix1(1.0f);
        cout << to_string(matrix1) << endl;
        mat3 matrix2(1.0f);
        cout << to_string(matrix2) << endl;

        mat3 matrix3(1.0f, 2.0f, 1.0f,
                     2.0f, 3.0f, 2.0f,
                     1.0f, 2.0f, 1.0f);
        cout << to_string(matrix3) << endl;
        mat3 matrix4(1.0f, 2.0f, 1.0f,
                     2.0f, 3.0f, 2.0f,
                     1.0f, 2.0f, 1.0f);
        cout << to_string(matrix4) << endl;

        HEADER("Test 1")
        exercise3(matrix1, matrix2);
        HEADER("Test 2")

        try {
            exercise3(matrix3, matrix4);

            cout << "Should've thrown an error";
        } catch(char const* message) {
            cout << "Success";
        }

    } catch (char const* message) {
        cout << "Unexpected error: " << message << endl;
    }
}


