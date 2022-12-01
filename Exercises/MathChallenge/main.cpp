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

// Exercise 1
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

    if(floatEquals(length(w), 0)) {
        // View and Up are collinear
        throw "The given View and Up vector must not be collinear";
    }

    w = w / length(w);

    vec3 u = cross(v, w);

    return mat3(v, u, w);
}

// Exercise 2
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

// Exercise 3
void exercise3(mat3 matrix1, mat3 matrix2) {
    if(floatEquals(determinant(matrix1), 0)) {
        throw "The determinant for the first matrix can't be 0";
    }

    if(floatEquals(determinant(matrix2), 0)) {
        throw "The determinant for the second matrix can't be 0";
    }

    mat3 cal1 = inverse(matrix1 * matrix2);
    mat3 cal2 = inverse(matrix2) * inverse(matrix1);
    float f1 = determinant(cal1);
    float f2 = determinant(cal2);

    assert(floatEquals(f1, f2));
}

void printDifferences(mat3 expected, mat3 obtained) {
    cout << "Test failed. Expected: " << to_string(expected) << ", obtained: " << to_string(obtained) << endl;
}

void printDifferencesVec3(vec3 expected, vec3 obtained) {
    cout << "Test failed. Expected: " << to_string(expected) << ", obtained: " << to_string(obtained) << endl;
}

int main() {

    try {
        // Exercise 1

        HEADER("Exercise 1 tests")

        vec3 view(2.0f, 0, 0);
        vec3 up(0.5f, 5.0f, 0);
        mat3 result;

        mat3 correctResult(1.0f, 0, 0,
                           0, 1, 0,
                           0, 0, -1);

        try {
            cout << "Test 1.1: A view on the x axis, an up on the y axis slightly tilted to x" << endl;

            result = createCoordinateFrame(view, up);

            if(floatsEquals(result, correctResult)) {
                cout << "Success" << endl;
            }
            else {
                printDifferences(correctResult, result);
            }
        } catch(char const* message) {
            cout << "Shouldn't have thrown an error: " << message << endl;
        }

        view = vec3(0, 1.0f, 0);
        up = vec3(0, 0.5f, 0.5f);

        correctResult = mat3(0, 1.0f, 0,
                            0, 0, 1.0f,
                            -1.0f, 0, 0);

        try {
            cout << "Test 1.2: A view on the y axis, an up angled 45 degrees between y and z" << endl;

            result = createCoordinateFrame(view, up);

            if(floatsEquals(result, correctResult)) {
                cout << "Success" << endl;
            }
            else {
                printDifferences(correctResult, result);
            }
        } catch(char const* message) {
            cout << "Shouldn't have thrown an error: " << message << endl;
        }

        view = vec3(1.0f, 0, 0);
        up = vec3(0, 0, 1.0f);

        correctResult = mat3(1.0f, 0, 0,
                             0, 0, 1.0f,
                             0, 1.0f, 0);

        try {
            cout << "Test 1.3: A view on the x axis, an up on the z axis" << endl;

            result = createCoordinateFrame(view, up);

            if(floatsEquals(result, correctResult)) {
                cout << "Success" << endl;
            }
            else {
                printDifferences(correctResult, result);
            }
        } catch(char const* message) {
            cout << "Shouldn't have thrown an error: " << message << endl;
        }

        view = vec3(-1.0f, 0, 0);
        up = vec3(0, 0, -1.0f);

        correctResult = mat3(-1.0f, 0, 0,
                             0, 0, -1.0f,
                             0, 1.0f, 0);

        try {
            cout << "Test 1.4: A view on the x axis, negative, and an up on the z axis, negative" << endl;

            result = createCoordinateFrame(view, up);

            if (floatsEquals(result, correctResult)) {
                cout << "Success" << endl;
            } else {
                printDifferences(correctResult, result);
            }
        } catch(char const* message) {
            cout << "Shouldn't have thrown an error: " << message << endl;
        }

        view = vec3(0, 0.1f, 0);
        up = vec3(10.0f, 0, 0);

        correctResult = mat3(0, 1.0f, 0,
                             1.0f, 0, 0,
                             0, 0, 1.0f);

        try {
            cout << "Test 1.5: A small view and a big up" << endl;

            result = createCoordinateFrame(view, up);

            if (floatsEquals(result, correctResult)) {
                cout << "Success" << endl;
            } else {
                printDifferences(correctResult, result);
            }
        } catch(char const* message) {
            cout << "Shouldn't have thrown an error: " << message << endl;
        }

        view = vec3(0.5f, 0.5f, 0);
        up = vec3(0, 0.5f, 0.5f);

        correctResult = mat3(0.70711f, 0.70711f, 0,
                             -0.40825f, 0.40825f, 0.8165f,
                             -0.57735f, 0.57735f, -0.57735f);

        try {
            cout << "Test 1.6: Diagonal view and up" << endl;

            result = createCoordinateFrame(view, up);

            if (floatsEquals(result, correctResult)) {
                cout << "Success" << endl;
            } else {
                printDifferences(correctResult, result);
            }
        } catch(char const* message) {
            cout << "Shouldn't have thrown an error: " << message << endl;
        }

        view = vec3(0.0f, 0, 0);
        up = vec3(1.0f, 0, 0);

        try {
            cout << "Test 1.7: Passing zero-length view" << endl;

            createCoordinateFrame(view, up);

            cout << "Should've thrown an error" << endl;
        } catch(char const* message) {
            cout << "Success" << endl;
        }

        view = vec3(1.0f, 0, 0);
        up = vec3(0.0f, 0, 0);

        try {
            cout << "Test 1.8: Passing zero-length up" << endl;

            createCoordinateFrame(view, up);

            cout << "Should've thrown an error" << endl;
        } catch(char const* message) {
            cout << "Success" << endl;
        }

        view = vec3(1.0f, 0, 0);
        up = vec3(2.0f, 0, 0);

        try {
            cout << "Test 1.9: Passing collinear vectors" << endl;

            createCoordinateFrame(view, up);

            cout << "Should've thrown an error" << endl;
        } catch(char const* message) {
            cout << "Success" << endl;
        }

        view = vec3(1.0f, 0, 0);
        up = vec3(-1.0f, 0, 0);

        try {
            cout << "Test 1.10: Passing collinear opposite direction vectors" << endl;

            createCoordinateFrame(view, up);

            cout << "Should've thrown an error" << endl;
        } catch(char const* message) {
            cout << "Success" << endl;
        }

        // Exercise 2

        HEADER("Exercise 2 tests")

        vec3 exampleX(2.0f, 0.0f, 0.0f);
        vec3 exampleY(0.0f, 2.0f, 0.0f);
        vec3 exampleZ(0.0f, 0.0f, 2.0f);

        vec3 example0(1.0f, 1.0f, 1.0f);
        vec3 example1(1.0f, 0.0f, 1.0f);
        vec3 example2(0.0f, 1.0f, 1.0f);

        vec3 rotationAxisX(1.0f, 0.0f, 0.0f);
        vec3 rotationAxisY(0.0f, 1.0f, 0.0f);
        vec3 rotationAxisZ(0.0f, 0.0f, 1.0f);

        vec3 resultVec3;

        try {
            cout << "Test 2.1: A rotation axis on x, 90 degrees, to vector (2,0,0)" << endl;

            mat3 rotationMatrix = rodriguesFormula(radians(90.0f), rotationAxisX);
            resultVec3 = rotationMatrix * exampleX;

            if(floatsEquals(exampleX, resultVec3)) {
                cout << "Success" << endl;
            } else {
                printDifferencesVec3(exampleX, resultVec3);
            }
        } catch(char const* message) {
            cout << "Shouldn't have thrown an error: " << message << endl;
        }

        try {
            cout << "Test 2.2: A rotation axis on y, 90 degrees, to vector (0,2,0)" << endl;

            mat3 rotationMatrix = rodriguesFormula(radians(90.0f), rotationAxisY);
            resultVec3 = rotationMatrix * exampleY;

            if(floatsEquals(exampleY, resultVec3)) {
                cout << "Success" << endl;
            } else {
                printDifferencesVec3(exampleY, resultVec3);
            }
        } catch(char const* message) {
            cout << "Shouldn't have thrown an error: " << message << endl;
        }

        try {
            cout << "Test 2.3: A rotation axis on vector(0,0,0), 90 degrees, to vector (0,0,2)" << endl;

            mat3 rotationMatrix = rodriguesFormula(radians(90.0f), vec3(0,0,0));
            resultVec3 = rotationMatrix * exampleZ;

            if(floatsEquals(exampleZ, resultVec3)) {
                cout << "Should've thrown an error" << endl;

            }
        } catch(char const* message) {
            cout << "Success" << endl;
        }

        try {
            cout << "Test 2.4: A rotation axis on x, 180 degrees, to vector (0,2,0)" << endl;

            mat3 rotationMatrix = rodriguesFormula(radians(180.0f), rotationAxisX);
            resultVec3 = rotationMatrix * exampleY;
            vec3 expectedResult(0.0f, -2.0f, 0.0f);

            if(floatsEquals(expectedResult, resultVec3)) {
                cout << "Success" << endl;
            } else {
                printDifferencesVec3(expectedResult, resultVec3);
            }
        } catch(char const* message) {
            cout << "Shouldn't have thrown an error: " << message << endl;
        }

        try {
            cout << "Test 2.5: A rotation axis on y, 90 degrees, to vector (0,0,2)" << endl;

            mat3 rotationMatrix = rodriguesFormula(radians(90.0f), rotationAxisY);
            resultVec3 = rotationMatrix * exampleZ;
            vec3 expectedResult(2.0f, 0.0f, 0.0f);

            if(floatsEquals(expectedResult, resultVec3)) {
                cout << "Success" << endl;
            } else {
                printDifferencesVec3(expectedResult, resultVec3);
            }
        } catch(char const* message) {
            cout << "Shouldn't have thrown an error: " << message << endl;
        }

        try {
            cout << "Test 2.6: A rotation axis on z, 270 degrees, to vector (2,0,0)" << endl;

            mat3 rotationMatrix = rodriguesFormula(radians(270.0f), rotationAxisZ);
            resultVec3 = rotationMatrix * exampleX;
            vec3 expectedResult(0.0f, -2.0f, 0.0f);

            if(floatsEquals(expectedResult, resultVec3)) {
                cout << "Success" << endl;
            } else {
                printDifferencesVec3(expectedResult, resultVec3);
            }
        } catch(char const* message) {
            cout << "Shouldn't have thrown an error: " << message << endl;
        }

        try {
            cout << "Test 2.7: A rotation axis on y, 45 degrees, to vector (1,1,1)" << endl;

            mat3 rotationMatrix = rodriguesFormula(radians(45.0f), rotationAxisY);
            resultVec3 = rotationMatrix * example0;
            vec3 expectedResult(1.414214f, 1.0f, 0.0f);

            if(floatsEquals(expectedResult, resultVec3)) {
                cout << "Success" << endl;
            } else {
                printDifferencesVec3(expectedResult, resultVec3);
            }
        } catch(char const* message) {
            cout << "Shouldn't have thrown an error: " << message << endl;
        }

        try {
            cout << "Test 2.8: A rotation axis on vector (1,1,1), 120 degrees, to vector (2,0,0)" << endl;

            mat3 rotationMatrix = rodriguesFormula(radians(120.0f), example0);
            resultVec3 = rotationMatrix * exampleX;
            vec3 expectedResult(0.0f, 2.0f, 0.0f);

            if(floatsEquals(expectedResult, resultVec3)) {
                cout << "Success" << endl;
            } else {
                printDifferencesVec3(expectedResult, resultVec3);
            }
        } catch(char const* message) {
            cout << "Shouldn't have thrown an error: " << message << endl;
        }

        try {
            cout << "Test 2.9: A rotation axis on vector (1,0,1), -180 degrees, to vector (0,0,2)" << endl;

            mat3 rotationMatrix = rodriguesFormula(radians(-180.0f), example1);
            resultVec3 = rotationMatrix * exampleZ;
            vec3 expectedResult(2.0f, 0.0f, 0.0f);

            if(floatsEquals(expectedResult, resultVec3)) {
                cout << "Success" << endl;
            } else {
                printDifferencesVec3(expectedResult, resultVec3);
            }
        } catch(char const* message) {
            cout << "Shouldn't have thrown an error: " << message << endl;
        }

        try {
            cout << "Test 2.10: A rotation axis on vector (1,1,1), -120 degrees, to vector (0,1,1)" << endl;

            mat3 rotationMatrix = rodriguesFormula(radians(-120.0f), example0);
            resultVec3 = rotationMatrix * example2;
            vec3 expectedResult(1.0f, 1.0f, 0.0f);

            if(floatsEquals(expectedResult, resultVec3)) {
                cout << "Success" << endl;
            } else {
                printDifferencesVec3(expectedResult, resultVec3);
            }
        } catch(char const* message) {
            cout << "Shouldn't have thrown an error: " << message << endl;
        }

        // Exercise 3

        HEADER("Exercise 3 tests")

        mat3 identity(1.0f);

        // Matrix with determinant equal to 0
        mat3 matrix0(1.0f, 2.0f, 1.0f,
                     2.0f, 3.0f, 2.0f,
                     1.0f, 2.0f, 1.0f);

        // Random matrix with positive determinant 1
        mat3 matrixp1(4.0f, -1.0f, 1.0f,
                     4.0f, 5.0f, 3.0f,
                     -2.0f, 0.0f, 0.0f);

        // Random matrix with positive determinant 2
        mat3x3 matrixp2(4.0f, -1.0f, 1.0f,
                       4.0f, 5.0f, 3.0f,
                       -3.0f, 2.0f, 5.0f);

        // Random matrix with negative determinant 1
        mat3 matrixn1(1.0f, 2.0f, 3.0f,
                     2.0f, 3.0f, 2.0f,
                     3.0f, 2.0f, 1.0f);

        // Random matrix with negative determinant 2
        mat3 matrixn2(1.0f, 2.0f, 3.0f,
                     3.0f, 3.0f, -2.0f,
                     3.0f, 2.0f, 1.0f);

        try {
            cout << "Test 3.1: 2 identity matrices" << endl;
            exercise3(identity, identity);

            cout << "Success" << endl;
        } catch(char const* message) {
            cout << "Shouldn't have thrown an error: " << message << endl;
        }

        try {
            cout << "Test 3.2: 1 matrix with determinant 0 and 1 matrix with determinant equal to 0" << endl;
            exercise3(matrix0, identity);

            cout << "Should've thrown an error" << endl;
        } catch(char const* message) {
            cout << "Success" << endl;
        }

        try {
            cout << "Test 3.3: 1 identity matrix and 1 matrix with determinant equal to 0" << endl;
            exercise3(identity, matrix0);

            cout << "Should've thrown an error" << endl;
        } catch(char const* message) {
            cout << "Success" << endl;
        }

        try {
            cout << "Test 3.4: 1 matrix with positive determinant and 1 identity matrix" << endl;
            exercise3(matrixp1, identity);

            cout << "Success" << endl;
        } catch(char const* message) {
            cout << "Shouldn't have thrown an error: " << message << endl;
        }

        try {
            cout << "Test 3.5: 1 matrix with negative determinant and 1 identity matrix" << endl;
            exercise3(matrixn1, identity);

            cout << "Success" << endl;
        } catch(char const* message) {
            cout << "Shouldn't have thrown an error: " << message << endl;
        }

        try {
            cout << "Test 3.6: 1 matrix with positive determinant and 1 matrix with negative determinant" << endl;
            exercise3(matrixp1, matrixn1);

            cout << "Success" << endl;
        } catch(char const* message) {
            cout << "Shouldn't have thrown an error: " << message << endl;
        }

        try {
            cout << "Test 3.7: 1 matrix with positive determinant and 1 matrix with negative determinant" << endl;
            exercise3(matrixp1, matrixn2);

            cout << "Success" << endl;
        } catch(char const* message) {
            cout << "Shouldn't have thrown an error: " << message << endl;
        }

        try {
            cout << "Test 3.8: 2 matrix with positive determinant" << endl;
            exercise3(matrixp1, matrixp2);

            cout << "Success" << endl;
        } catch(char const* message) {
            cout << "Shouldn't have thrown an error: " << message << endl;
        }

        try {
            cout << "Test 3.9: 1 matrix with positive determinant and 1 matrix with negative determinant" << endl;
            exercise3(matrixp2, matrixn1);

            cout << "Success" << endl;
        } catch(char const* message) {
            cout << "Shouldn't have thrown an error: " << message << endl;
        }

        try {
            cout << "Test 3.10: 1 matrix with positive determinant and 1 matrix with negative determinant" << endl;
            exercise3(matrixp2, matrixn2);

            cout << "Success" << endl;
        } catch(char const* message) {
            cout << "Shouldn't have thrown an error: " << message << endl;
        }

    } catch (char const* message) {
        cout << "Unexpected error: " << message << endl;
    }
}


