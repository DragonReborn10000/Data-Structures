// Name: AbdulMalik Shodunke

#include <iostream>
using namespace std;

class SparseRow {
protected:
    int row;
    int col;
    int value;

public:
    SparseRow() : row(-1), col(-1), value(0) {}

    //display
    void display() const {
        cout << "Row: " << row << ", Col: " << col << ", Value: " << value << endl;
    }

    //other methods that are necessary such as get and set
    void setRow(int r) { row = r; }
    void setCol(int c) { col = c; }
    void setValue(int v) { value = v; }

    int getRow() const { return row; }
    int getCol() const { return col; }
    int getValue() const { return value; }

    // Overload << operator to print the sparse row in the format: row, col, value
    friend ostream& operator<<(ostream& os, const SparseRow& sr) {
        os << sr.row << ", " << sr.col << ", " << sr.value;
        return os;
    }
};

class SparseMatrix {
protected:
    int noRows; //Number of rows of the original matrix
    int noCols; //Number of columns of the original matrix
    int commonValue; // Value of the zeros
    int nonSparseValues; // Number of non-zeros
    SparseRow* myMatrix; // Array to store non-zero values

public:
    SparseMatrix() : noRows(0), noCols(0), commonValue(0) {}

    SparseMatrix(int rows, int cols, int defVal, int nonZeros)
        : noRows(rows), noCols(cols), commonValue(defVal), nonSparseValues(nonZeros) {
        myMatrix = new SparseRow[nonSparseValues]; // One dimensional array
    }

    ~SparseMatrix() {
        delete[] myMatrix; // delete dynamically allocated memory
    }

    SparseRow* getSparseRows() {
        return myMatrix;
    }

    SparseMatrix* transpose() {
        SparseMatrix* transposedMatrix = new SparseMatrix(noCols, noRows, commonValue, nonSparseValues);
        for (int i = 0; i < nonSparseValues; i++) {
            SparseRow original = myMatrix[i];
            int row = original.getRow();
            int col = original.getCol();
            int value = original.getValue();

            SparseRow transposedRow;
            transposedRow.setRow(col);
            transposedRow.setCol(row);
            transposedRow.setValue(value);
            transposedMatrix->getSparseRows()[i] = transposedRow;
        }
        return transposedMatrix;
    }

    SparseMatrix* Multiply(const SparseMatrix& other) const {
        if (noCols != other.noRows) {
            return nullptr; // If multiplication is not possible
        }

        SparseMatrix* product = new SparseMatrix(noRows, other.noCols, commonValue, nonSparseValues * other.nonSparseValues);
        int currentIdx = 0;

        for (int i = 0; i < noRows; i++) {
            for (int j = 0; j < other.noCols; j++) {
                int sum = 0;
                for (int k = 0; k < noCols; k++) {
                    int value1 = 0, value2 = 0;
                    for (int a = 0; a < nonSparseValues; a++) {
                        if (myMatrix[a].getRow() == i && myMatrix[a].getCol() == k) {
                            value1 = myMatrix[a].getValue();
                            break;
                        }
                    }
                    for (int b = 0; b < other.nonSparseValues; b++) {
                        if (other.myMatrix[b].getRow() == k && other.myMatrix[b].getCol() == j) {
                            value2 = other.myMatrix[b].getValue();
                            break;
                        }
                    }
                    sum += value1 * value2;
                }
                if (sum != 0) {
                    product->myMatrix[currentIdx].setRow(i);
                    product->myMatrix[currentIdx].setCol(j);
                    product->myMatrix[currentIdx].setValue(sum);
                    currentIdx++;
                }
            }
        }

        product->nonSparseValues = currentIdx;
        return product;
    }

    SparseMatrix* add(const SparseMatrix& other) const {
        if (noRows != other.noRows || noCols != other.noCols) {
            return nullptr; // If addition is nt possible
        }

        // Creates new SparseMatrix for the result with proper size
        SparseMatrix* MatrixSum = new SparseMatrix(noRows, noCols, commonValue, nonSparseValues + other.nonSparseValues);
        SparseRow* result = MatrixSum->getSparseRows();
        int resIndex = 0;

        int a = 0, b = 0;
        while (a < nonSparseValues && b < other.nonSparseValues) {
            int rowA = myMatrix[a].getRow();
            int colA = myMatrix[a].getCol();
            int valueA = myMatrix[a].getValue();
            int rowB = other.myMatrix[b].getRow();
            int colB = other.myMatrix[b].getCol();
            int valueB = other.myMatrix[b].getValue();

            if (rowA == rowB && colA == colB) {
                int totalValue = valueA + valueB;
                if (totalValue != commonValue) {
                    result[resIndex].setRow(rowA);
                    result[resIndex].setCol(colA);
                    result[resIndex].setValue(totalValue);
                    resIndex++;
                }
                a++;
                b++;
            }
            else if (rowA < rowB || (rowA == rowB && colA < colB)) {
                result[resIndex] = myMatrix[a];
                a++;
                resIndex++;
            }
            else {
                result[resIndex] = other.myMatrix[b];
                b++;
                resIndex++;
            }
        }

        while (a < nonSparseValues) {
            result[resIndex] = myMatrix[a];
            a++;
            resIndex++;
        }

        while (b < other.nonSparseValues) {
            result[resIndex] = other.myMatrix[b];
            b++;
            resIndex++;
        }

        MatrixSum->nonSparseValues = resIndex;
        return MatrixSum;
    }

    friend ostream& operator<<(ostream& s, const SparseMatrix& sm) {
        for (int i = 0; i < sm.nonSparseValues; i++) {
            s << sm.myMatrix[i] << endl;
        }
        return s;
    }

    void printSparse() {
        for (int i = 0; i < nonSparseValues; i++) {
            cout << myMatrix[i] << endl;
        }
    }

    // Display the matrix in its original format
    void printMatrix() {
        int pos = 0;
        for (int i = 0; i < noRows; i++) {
            for (int j = 0; j < noCols; j++) {
                if (pos < nonSparseValues && myMatrix[pos].getRow() == i && myMatrix[pos].getCol() == j) {
                    cout << myMatrix[pos].getValue() << " ";
                    pos++;
                }
                else {
                    cout << commonValue << " ";
                }
            }
            cout << endl;
        }
    }
};

//define all your classes here (as given above)
//write the methods after the class definition

int main() {
    int n, m, cv, noNSV;

    //First input
    cin >> n >> m >> cv >> noNSV;
    SparseMatrix* matrixA = new SparseMatrix(n, m, cv, noNSV);
    SparseRow* rowsA = matrixA->getSparseRows();

    int value, idx = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> value;
            if (value != cv) {
                rowsA[idx].setRow(i);
                rowsA[idx].setCol(j);
                rowsA[idx].setValue(value);
                idx++;
            }
        }
    }

    // 2nd input
    cin >> n >> m >> cv >> noNSV;
    SparseMatrix* matrixB = new SparseMatrix(n, m, cv, noNSV);
    SparseRow* rowsB = matrixB->getSparseRows();

    idx = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> value;
            if (value != cv) {
                rowsB[idx].setRow(i);
                rowsB[idx].setCol(j);
                rowsB[idx].setValue(value);
                idx++;
            }
        }
    }


    // Most outputs
    // Show Sparsed First matrix
    cout << "First one in sparse matrix format" << endl;
    matrixA->printSparse();

    // Show Transpose of Sparsed First matrix
    cout << "After transpose" << endl;
    SparseMatrix* transposedA = matrixA->transpose();
    transposedA->printSparse();

    // Show normal First matrix
    cout << "First one in matrix format" << endl;
    matrixA->printMatrix();

    // Show Sparsed Second Matrix
    cout << "Second one in sparse matrix format" << endl;
    matrixB->printSparse();

    // Show Transpose of Sparsed Second Matrix
    cout << "After transpose" << endl;
    SparseMatrix* transposedB = matrixB->transpose();
    transposedB->printSparse();

    // Show normal First Matrix
    cout << "Second one in matrix format" << endl;
    matrixB->printMatrix();


    cout << "Matrix addition result" << endl;
    SparseMatrix* sumResult = matrixA->add(*matrixB);
    if (sumResult != nullptr) {
        sumResult->printMatrix();
    }
    else {
        cout << "Matrix addition is not possible" << endl;
    }

    cout << "Matrix multiplication result" << endl;
    SparseMatrix* productResult = matrixA->Multiply(*matrixB);
    if (productResult != nullptr) {
        productResult->printMatrix();
    }
    else {
        cout << "Matrix multiplication is not possible" << endl;
    }

    delete matrixA;
    delete matrixB;
    delete transposedA;
    delete transposedB;
    delete sumResult;
    delete productResult;

    return 0;
}


/*
Name: AbdulMalik Shodunke

// Prompts Asked to GPT:
1. I asked GPT for a breakdown of the starter code provided, including explanations for the SparseRow and SparseMatrix classes and how to approach the implementation of matrix operations like transpose, addition, and multiplication.
2. I requested guidance on how to handle input/output, particularly how to display the sparse and full matrix formats.
3. I asked how to handle dynamic memory allocation and cleanup to prevent memory leaks.
4. I inquired about the best way to implement operator overloading for printing sparse matrix elements concisely.


// Rationale:
1. The SparseRow class was used to represent individual non-zero matrix elements, while the SparseMatrix class managed the overall structure and operations.
2. Using dynamic memory allocation for the sparse matrix allowed flexible storage of non-zero elements. However, I needed to ensure proper memory management, especially when deleting objects.
3. Matrix operations like transpose, addition, and multiplication were implemented with a focus on efficient handling of sparse elements to reduce unnecessary computations on zero values.
4. I used operator overloading (`<<`) for the SparseRow class to make printing easier and more readable, and a separate print method for displaying the entire matrix in its original format.


// Approach and Challenges:
1. Transpose: I asked GPT how to swap rows and columns for the transpose operation. Initially, I swapped rows and columns directly, but GPT suggested storing the transposed elements in a new matrix, which avoided modifying the original matrix.
2. Addition: I encountered issues with non-matching rows and columns when adding matrices. GPT explained how to handle this by iterating over both sparse matrices and merging common elements. This allowed me to successfully combine the two matrices without losing non-zero values.
3. Multiplication: GPT's explanation helped me realize that matrix multiplication involves the dot product of corresponding rows and columns. The challenge was finding non-zero values in the correct positions. I initially failed to handle this properly, but with GPT's advice, I added a nested loop to check each element, which solved the problem.
4. Display: GPT suggested using operator overloading for concise printing of the sparse matrix and a separate function for the full matrix. This led to cleaner and more efficient code.


// What I Tried and Failed:
1. Addition: I initially tried adding sparse matrices by directly iterating over both arrays. This failed when the indices didn't match between matrices. After discussing with GPT, I realized I needed to compare the row and column indices and merge the two matrices accordingly.
2. Multiplication: My first attempt at multiplication treated it like addition, but GPT explained that matrix multiplication requires a more complex approach with the dot product. I initially struggled with handling this but managed to fix it by following GPT’s step-by-step guidance on matrix multiplication.
3. Memory management: I asked GPT about preventing memory leaks with dynamic allocation, and I learned to implement destructors for cleaning up dynamically allocated arrays.


// Lessons Learned:
1. Matrix operations on sparse matrices require careful indexing and tracking of non-zero values to avoid unnecessary computations.
2. Operator overloading improves the readability of the code, especially for tasks like printing sparse matrix elements.
3. Understanding how to handle dynamic memory correctly is essential for managing large data structures like matrices.
4. Collaboration with GPT helped me identify and fix issues I initially overlooked, like matrix compatibility checks for addition and multiplication.
5. Implementing matrix operations in both sparse and full formats made it easier to test and visualize the results, ensuring correctness.
*/
