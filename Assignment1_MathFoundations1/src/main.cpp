// Includes for the assignment
#include "Vector4f.h"
#include "Matrix4f.h"
#include <iostream>

// Tests for comparing our library
// You may compare your operations against the glm library
// which is a well tested library.
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

bool areVectorsEqual(Vector4f a, glm::vec4 b) {
  return
    (a.x == b.x) &&
    (a.y == b.y) &&
    (a.z == b.z) &&
    (a.w == b.w);
}

bool areMatricesEqual(Matrix4f A, glm::mat4 B) {
  return
    areVectorsEqual(A[0], B[0]) &&
    areVectorsEqual(A[1], B[1]) &&
    areVectorsEqual(A[2], B[2]) &&
    areVectorsEqual(A[3], B[3]);
}

// Sample unit test comparing against GLM.
bool unitTest0() {
  Matrix4f myIdentity(
    1.0f, 0, 0, 0,
    0, 1.0f, 0, 0,
    0, 0, 1.0f, 0,
    0, 0, 0, 1.0f
  );
  glm::mat4 glmIdentityMatrix = glm::mat4(1.0f);

  return areMatricesEqual(myIdentity, glmIdentityMatrix);
}

bool unitTest1() {
  Matrix4f myIdentity(
    1.0f, 0, 0, 0,
    0, 1.0f, 0, 0,
    0, 0, 1.0f, 0,
    0, 0, 0, 1.0f
  );
  glm::mat4 glmIdentityMatrix = glm::mat4(1.0f);

  // Not using areMatricesEqual since we're specifically
  // testing the () operator.
  if (
    glmIdentityMatrix[0][0] == myIdentity(0, 0) &&
    glmIdentityMatrix[0][1] == myIdentity(0, 1) &&
    glmIdentityMatrix[0][2] == myIdentity(0, 2) &&
    glmIdentityMatrix[0][3] == myIdentity(0, 3) &&
    glmIdentityMatrix[1][0] == myIdentity(1, 0) &&
    glmIdentityMatrix[1][1] == myIdentity(1, 1) &&
    glmIdentityMatrix[1][2] == myIdentity(1, 2) &&
    glmIdentityMatrix[1][3] == myIdentity(1, 3) &&
    glmIdentityMatrix[2][0] == myIdentity(2, 0) &&
    glmIdentityMatrix[2][1] == myIdentity(2, 1) &&
    glmIdentityMatrix[2][2] == myIdentity(2, 2) &&
    glmIdentityMatrix[2][3] == myIdentity(2, 3) &&
    glmIdentityMatrix[3][0] == myIdentity(3, 0) &&
    glmIdentityMatrix[3][1] == myIdentity(3, 1) &&
    glmIdentityMatrix[3][2] == myIdentity(3, 2) &&
    glmIdentityMatrix[3][3] == myIdentity(3, 3)) {
    return true;
  }

  return false;
}

// Sample unit test comparing against GLM.
bool unitTest2() {
  Vector4f a(1.0f, 0.0f, 0.0f, 0.0f);
  Vector4f b(0.0f, 1.0f, 0.0f, 0.0f);
  Vector4f c(0.0f, 0.0f, 1.0f, 0.0f);
  Vector4f d(0.0f, 0.0f, 0.0f, 1.0f);
  Matrix4f myIdentity(a, b, c, d);
  glm::mat4 glmIdentityMatrix = glm::mat4(1.0f);

  return areMatricesEqual(myIdentity, glmIdentityMatrix);
}

// Sample unit test comparing against GLM.
bool unitTest3() {
  Matrix4f myScaled;
  myScaled = myScaled.MakeScale(2.0f, 2.0f, 2.0f);
  glm::mat4 glmScale = glm::scale(glm::vec3(2.0f, 2.0f, 2.0f));

  return areMatricesEqual(myScaled, glmScale);
}

// Sample unit test comparing against GLM.
// Testing operator
bool unitTest4() {
  Matrix4f myMatrix(
    1.0f, 0, 0, 0,
    0, 1.0f, 0, 0,
    0, 0, 1.0f, 0,
    0, 0, 0, 1.0f
  );
  myMatrix[1][3] = 72.0f;
  myMatrix[2][3] = 2.1f;

  glm::mat4 glmTest = glm::mat4(1.0f);
  glmTest[1][3] = 72.0f;
  glmTest[2][3] = 2.1f;

  return areMatricesEqual(myMatrix, glmTest);
}

// Sample unit test testing your library
bool unitTest5() {
  Vector4f a = Vector4f(1, 1, 1, 1) + Vector4f(0, 0, 0, 0);
  return (a.x == 1 && a.y == 1 && a.z == 1 && a.w == 1);
}

bool unitTest6() {
  bool result = true;
  Vector4f a = Vector4f(1, 2, 3, 4);
  Vector4f b = Vector4f(5, 6, 7, 8);

  a *= 2;
  result = result && (a.x == 2 && a.y == 4 && a.z == 6 && a.w == 8);

  a /= 2;
  result = result && (a.x == 1 && a.y == 2 && a.z == 3 && a.w == 4);

  a += b;
  result = result && (a.x == 6 && a.y == 8 && a.z == 10 && a.w == 12);

  a -= b;
  result = result && (a.x == 1 && a.y == 2 && a.z == 3 && a.w == 4);

  return result;
}

bool unitTest7() {
  bool result = true;
  Vector4f a = Vector4f(1, 2, 3, 4);
  Vector4f b;

  b = a * 2;
  result = result && (b.x == 2 && b.y == 4 && b.z == 6 && b.w == 8);

  b = a / 2;
  result = result && (b.x == 0.5f && b.y == 1.0f && b.z == 1.5f && b.w == 2.0f);

  b = -a;
  result = result && (b.x == -1 && b.y == -2 && b.z == -3 && b.w == -4);

  return result;
}

bool unitTest8() {
  Vector4f a = Vector4f(1, 2, 3, 4);
  Vector4f b = Vector4f(5, 6, 7, 8);

  glm::vec4 glmA = glm::vec4(1, 2, 3, 4);
  glm::vec4 glmB = glm::vec4(5, 6, 7, 8);

  return (
    areVectorsEqual(a + b, glmA + glmB) &&
    areVectorsEqual(b - a, glmB - glmA));
}

bool unitTest9() {
  bool result = true;
  Vector4f a(1, 2, 3, 4);
  Vector4f b(5, 6, 7, 8);

  result = result && (Dot(a, b) == 70);
  result = result && (Magnitude(b) - 13.1909f < 0.0001f);
  
  Vector4f c = CrossProduct(a, b);
  result = result && (c.x == -4 && c.y == 8 && c.z == -4 && c.w == 1);

  return result;
}

bool unitTest10() {
  Vector4f a = Matrix4f(
    1, 2, 3, 4,
    5, 6, 7, 8,
    9, 1, 2, 3,
    4, 5, 6, 7
  ) * Vector4f(3, 7, 2, 6);

  glm::vec4 b = glm::mat4(
    1, 5, 9, 4,
    2, 6, 1, 5,
    3, 7, 2, 6,
    4, 8, 3, 7
  ) * glm::vec4(3, 7, 2, 6);

  return areVectorsEqual(a, b);
}

bool unitTest11() {
  Matrix4f A = Matrix4f(
    1, 2, 3, 4,
    5, 6, 7, 8,
    9, 1, 2, 3,
    4, 5, 6, 7
  ) * Matrix4f(
    3, 4, 5, 6,
    7, 8, 9, 1,
    2, 3, 4, 5,
    6, 7, 8, 9
  );

  glm::mat4 B = glm::mat4(
    3, 4, 5, 6,
    7, 8, 9, 1,
    2, 3, 4, 5,
    6, 7, 8, 9
  ) * glm::mat4(
    1, 2, 3, 4,
    5, 6, 7, 8,
    9, 1, 2, 3,
    4, 5, 6, 7
  );
  
  return areMatricesEqual(A, B);
}

bool unitTest12() {
  Matrix4f A = Matrix4f(
    1.0f, 0, 0, 0,
    0, 1.0f, 0, 0,
    0, 0, 1.0f, 0,
    0, 0, 0, 1.0f
  );
  glm::mat4 B = glm::mat4(1.0f);

  Matrix4f xRotation = A * A.MakeRotationX(glm::radians(20.0f));
  glm::mat4 glmXRotation = glm::rotate(B, glm::radians(20.0f), glm::vec3(1, 0, 0));

  Matrix4f yRotation = A * A.MakeRotationY(glm::radians(20.0f));
  glm::mat4 glmYRotation = glm::rotate(B, glm::radians(20.0f), glm::vec3(0, 1, 0));

  Matrix4f zRotation = A * A.MakeRotationZ(glm::radians(20.0f));
  glm::mat4 glmZRotation = glm::rotate(B, glm::radians(20.0f), glm::vec3(0, 0, 1));

  return (
    areMatricesEqual(xRotation, glmXRotation) &&
    areMatricesEqual(yRotation, glmYRotation) &&
    areMatricesEqual(zRotation, glmZRotation));
}

int main() {
  // Run 'unit tests'
  std::cout << "Passed 0: " << unitTest0() << " \n";
  std::cout << "Passed 1: " << unitTest1() << " \n";
  std::cout << "Passed 2: " << unitTest2() << " \n";
  std::cout << "Passed 3: " << unitTest3() << " \n";
  std::cout << "Passed 4: " << unitTest4() << " \n";
  std::cout << "Passed 5: " << unitTest5() << " \n";
  std::cout << "Passed 6: " << unitTest6() << " \n";
  std::cout << "Passed 7: " << unitTest7() << " \n";
  std::cout << "Passed 8: " << unitTest8() << " \n";
  std::cout << "Passed 9: " << unitTest9() << " \n";
  std::cout << "Passed 10: " << unitTest10() << " \n";
  std::cout << "Passed 11: " << unitTest11() << " \n";
  std::cout << "Passed 12: " << unitTest12() << " \n";

  return 0;
}
