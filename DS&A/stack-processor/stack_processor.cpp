#include <iostream>
#include <string.h>
using namespace std;
struct CharNode {
    char data;
    CharNode* next;
};
struct StackElement {
    CharNode* data;
    StackElement* lower;
    StackElement* upper;
    StackElement() {
        lower = nullptr;
        upper = nullptr;
        data = nullptr;
    }
};
class Stack {
public:
    StackElement* top;
    Stack(){top = nullptr;};
    bool isEmpty() {
        return top == nullptr;
    }
    void push(CharNode* newData) {
        StackElement* newElement = new StackElement();
        newElement->data = newData;
        newElement->lower = top;

        if (top != nullptr) {
            top->upper = newElement;
        }

        top = newElement;
    }
    // " ' "
    void pushEmptyList() {
        CharNode* emptyNode = new CharNode();
        emptyNode->data = '\t';
        emptyNode->next = nullptr;

        push(emptyNode);
    }
    //","
    CharNode* pop() {
        if (isEmpty()) {
            return nullptr;
        }

        StackElement* temp = top;
        CharNode* poppedData = temp->data;

        top = top->lower;

        if (top != nullptr) {
            top->upper = nullptr;
        }

        delete temp;
        return poppedData;
    }
    // ":"
    void duplicateTop() {
        if (isEmpty()) {
            return;
        }

        CharNode* copiedList = copyListRecursive(top->data);
        push(copiedList);
    }
    // ";"
    void swapTop() {
        if (isEmpty() || top->lower == nullptr) {
            return;
        }

        StackElement* first = top;
        StackElement* second = top->lower;

        first->lower = second->lower;
        if (second->lower != nullptr) {
            second->lower->upper = first;
        }
        second->lower = first;
        second->upper = first->upper;
        first->upper = second;

        top = second;
    }
    // pozostale znaki
    void addChar(char c) {
        if (isEmpty()) {
            pushEmptyList();
        }

        CharNode* newNode = new CharNode;
        newNode->data = c;
        newNode->next = top->data;

        top->data = newNode;
    }
    // "&"
    void printCharNode(CharNode* node) {
        if (node == nullptr || node->data == '\0') {
            return;
        }
        cout << node->data;
        printCharNode(node->next);
    }
    void printStackElement(StackElement* current, int index) {
        if (current == nullptr) {
            return;
        }

        printStackElement(current->lower, index + 1);

        cout << index << ": ";
        CharNode* node = current->data;
        printCharNode(node);
        cout << endl;
    }
    void printStack() {
        if (isEmpty()) {
            return;
        }

        printStackElement(top, 0);
    }
    // "@"
    void pushA() {
        if (isEmpty()) {
            return;
        }

        CharNode* aNode = pop();
        if (aNode == nullptr) {
            return;
        }
        int A = buildNumberFromCharNodes(aNode);
        deleteList(aNode);

        int stackSize = countStackElements(top);
        if (stackSize <= A) {
            return;
        }

        StackElement* targetElement = getStackElementAtDepth(top, A);
        if (targetElement == nullptr) {
            return;
        }

        CharNode* copiedList = copyListRecursive(targetElement->data);
        push(copiedList);
    }
    int buildNumberFromCharNodes(CharNode* node, int multiplier = 1) {
        if (node->next == nullptr) {
            return 0;
        }

        int result = buildNumberFromCharNodes(node->next, multiplier * 10);

        int digit = node->data - '0';
        return result + digit * multiplier;
    }
    int countStackElements(StackElement* element, int count = 0) {
        if (element == nullptr) {
            return count;
        }
        return countStackElements(element->lower, count + 1);
    }
    StackElement* getStackElementAtDepth(StackElement* element, int depth) {
        if (element == nullptr || depth < 0) {
            return nullptr;
        }
        if (depth == 0) {
            return element;
        }
        return getStackElementAtDepth(element->lower, depth - 1);
    }
    CharNode* copyListRecursive(CharNode* original, CharNode* tail = nullptr) {
        if (original == nullptr) {
            return nullptr;
        }

        CharNode* newNode = new CharNode{original->data, nullptr};

        if (tail != nullptr) {
            tail->next = newNode;
        }

        copyListRecursive(original->next, newNode);
        return (tail == nullptr) ? newNode : tail;
    }
    // "."
    void appendCharTop() {
        if (isEmpty()) {
            return;
        }

        char c;
        cin >> c;

        CharNode* newNode = new CharNode();
        newNode->data = c;
        newNode->next = top->data;
        top->data = newNode;
    }
    //">"
    void printFirstAndPop() {
        if (isEmpty()) {
            return;
        }

        CharNode* topList = top->data;
        if (topList != nullptr) {
            cout << topList->data;
        }

        pop();
    }
    //"-" and "^"
    void negate() {
        if (isEmpty()) {
            return;
        }

        // Obsługa pustej listy
        if (top->data == nullptr || (top->data->data == '\t' && top->data->next == nullptr)) {
            CharNode* newMinus = new CharNode{'-', nullptr};
            CharNode* newSpace = new CharNode{'\t', nullptr};
            newMinus->next = newSpace;
            top->data = newMinus;
            return;
        }

        CharNode* prev = nullptr;
        CharNode* lastBeforeSpace = findLastBeforeSpace(top->data, prev);

        if (lastBeforeSpace->data == '-') {
            removeMinus(top->data, prev, lastBeforeSpace);
        } else {
            handleNegationCase(lastBeforeSpace, prev);
        }
    }
    void absolute() {
        if (isEmpty() || top->data == nullptr) {
            return;
        }

        CharNode* prev = nullptr;
        CharNode* prevBeforeSpace = nullptr;
        findLastAndPrev(top->data, prev, prevBeforeSpace);

        if (prev != nullptr && prev->data == '-') {
            if (!(prevBeforeSpace != nullptr && isdigit(prevBeforeSpace->data))) {
                removeMinus(top->data, prevBeforeSpace, prev);
            }
        }
    }
    CharNode* findLastBeforeSpace(CharNode* current, CharNode*& prev) {
        if (current == nullptr || current->data == '\t' ||
            (current->next != nullptr && current->next->data == '\t')) {
            return current;
        }
        prev = current;
        return findLastBeforeSpace(current->next, prev);
    }
    void findLastAndPrev(CharNode* current, CharNode*& prev, CharNode*& prevBeforeSpace) {
        if (current == nullptr || current->data == '\t') {
            return;
        }
        prevBeforeSpace = prev;
        prev = current;
        findLastAndPrev(current->next, prev, prevBeforeSpace);
    }
    static void removeMinus(CharNode*& listHead, CharNode* prev, CharNode* minusNode) {
        if (prev != nullptr) {
            prev->next = minusNode->next;
        } else {
            listHead = minusNode->next;
        }
        delete minusNode;
    }
    void handleNegationCase(CharNode* lastBeforeSpace, CharNode* prev) {
        if (lastBeforeSpace->data == '\t' && prev != nullptr && prev->data == '-' &&
            isdigit(lastBeforeSpace->next->data)) {
            addChar('-');
        } else {
            if (lastBeforeSpace->data == '\t') {
                lastBeforeSpace->data = '-';
            } else {
                lastBeforeSpace->next = new CharNode{'-', new CharNode{'\t', nullptr}};
            }
        }
    }
    //"$"
    void separate() {
        if (isEmpty() || top->data == nullptr) {
            return;
        }

        CharNode* firstChar = top->data;
        if (firstChar->data == '\t' && firstChar->next == nullptr) {
            return;
        }

        CharNode* remainingList = firstChar->next;
        firstChar->next = nullptr;

        top->data = remainingList;

        CharNode* newList = firstChar;
        CharNode* spaceNode = new CharNode{'\t', nullptr};
        newList->next = spaceNode;

        StackElement* newElement = new StackElement();
        newElement->data = newList;
        newElement->lower = top;
        top = newElement;
    }
    //"#"
    void popAndAddChar() {
        if (isEmpty() || top->lower == nullptr) {
            return;
        }

        StackElement* elementA = top;
        top = top->lower;
        CharNode* listA = elementA->data;
        delete elementA;

        CharNode* prev = nullptr;
        findSpaceAndAttach(top->data, prev, listA);

        if (prev == nullptr) {
            CharNode* endOfA = findEnd(listA);
            if (endOfA->next == nullptr) {
                endOfA->next = new CharNode{'\t', nullptr};
            }
            top->data = listA;
        }
    }
    void findSpaceAndAttach(CharNode* current, CharNode*& prev, CharNode* listA) {
        if (current == nullptr || current->data == '\t') {
            if (prev != nullptr) {
                prev->next = listA;
                CharNode* endOfA = findEnd(listA);
                endOfA->next = current;
            }
            return;
        }
        prev = current;
        findSpaceAndAttach(current->next, prev, listA);
    }
    CharNode* findEnd(CharNode* node) {
        if (node->next == nullptr) {
            return node;
        }
        return findEnd(node->next);
    }
    //"]"
    void popAndAddAscii() {
        if (isEmpty()) {
            return;
        }

        CharNode* numberList = pop();
        if (numberList == nullptr) {
            return;
        }

        int sign = 1;
        if (numberList->data == '-') {
            sign = -1;
            numberList = numberList->next;
        }

        int length = countDigits(numberList);
        int A = convertToReversedNumber(numberList, length, 1) * sign;

        deleteList(numberList);

        if (A >= 0 && A <= 127) {
            CharNode* newChar = new CharNode{static_cast<char>(A), new CharNode{'\t', nullptr}};
            push(newChar);
        }
    }
    int countDigits(CharNode* node, int count = 0) {
        if (node == nullptr || node->data == '\t') {
            return count;
        }
        if (isdigit(node->data)) {
            return countDigits(node->next, count + 1);
        }
        return countDigits(node->next, count);
    }
    int convertToReversedNumber(CharNode* node, int remainingDigits, int position) {
        if (node == nullptr || node->data == '\t' || remainingDigits == 0) {
            return 0;
        }
        if (isdigit(node->data)) {
            int digit = node->data - '0';
            return digit * position + convertToReversedNumber(node->next, remainingDigits - 1, position * 10);
        }
        return convertToReversedNumber(node->next, remainingDigits, position);
    }
    // "["
    void popAndAddNumeric() {
        if (isEmpty()) {
            return;
        }

        CharNode* listA = pop();
        if (listA == nullptr) {
            return;
        }

        CharNode* firstCharNode = findFirstNonSpaceChar(listA);
        if (firstCharNode == nullptr) {
            return;
        }

        int asciiValue = static_cast<int>(firstCharNode->data);
        CharNode* numericList = createReversedNumberList(asciiValue);
        push(numericList);

        deleteList(listA);
    }
    CharNode* findFirstNonSpaceChar(CharNode* node) {
        if (node == nullptr || node->data != '\t') {
            return node;
        }
        return findFirstNonSpaceChar(node->next);
    }
    CharNode* createReversedNumberList(int value, CharNode* tail = nullptr) {
        if (value == 0) {
            if (tail == nullptr) {
                return new CharNode{'0', new CharNode{'\t', nullptr}};
            }
            tail->next = new CharNode{'\t', nullptr};
            return nullptr;
        }

        int digit = value % 10;
        CharNode* newNode = new CharNode{static_cast<char>(digit + '0'), nullptr};

        if (tail != nullptr) {
            tail->next = newNode;
        }

        createReversedNumberList(value / 10, newNode);
        return (tail == nullptr) ? newNode : tail;
    }
    // "<"
    void popABBinaryLess() {
        if (isEmpty() || top->lower == nullptr) {
            return;
        }

        CharNode* listA = pop();
        CharNode* listB = pop();

        bool aOnlyZeros = containsOnlyZerosIgnoreSign(listA);
        bool bOnlyZeros = containsOnlyZerosIgnoreSign(listB);

        if (aOnlyZeros && bOnlyZeros) {
            push(createResultList('0'));
            deleteList(listA);
            deleteList(listB);
            return;
        }

        bool aNegative = isNegative(listA);
        bool bNegative = isNegative(listB);

        if (bNegative && !aNegative) {
            // B ujemne, A dodatnie -> B < A
            push(createResultList('1'));
            deleteList(listA);
            deleteList(listB);
            return;
        } else if (!bNegative && aNegative) {
            // B dodatnie, A ujemne -> B > A
            push(createResultList('0'));
            deleteList(listA);
            deleteList(listB);
            return;
        }
        if (containsOnlyZeros(listA) && containsOnlyZeros(listB)) {
            push(createResultList('0'));
            deleteList(listA);
            deleteList(listB);
            return;
        }

        if (aNegative) {
            removeSign(listA);
            removeSign(listB);
        }

        CharNode* reversedA = reverseList(listA);
        CharNode* reversedB = reverseList(listB);

        bool bLessA = compareLess(reversedB, reversedA, aNegative);

        push(createResultList(bLessA ? '1' : '0'));
        deleteList(listA);
        deleteList(listB);
        deleteList(reversedA);
        deleteList(reversedB);
    }
    bool containsOnlyZerosIgnoreSign(CharNode* node) {
        // Warunek kończący rekurencję
        if (node == nullptr || node->data == '\t') {
            return true;
        }
        if (node->data == '-') {
            return containsOnlyZerosIgnoreSign(node->next);
        }
        if (node->data != '0') {
            return false;
        }
        return containsOnlyZerosIgnoreSign(node->next);
    }
    bool compareLess(CharNode* first, CharNode* second, bool isNegative) {
        first = skipLeadingZeros(first);
        second = skipLeadingZeros(second);
        int lenFirst = countDigits(first);
        int lenSecond = countDigits(second);

        if (isNegative) {
            if (lenFirst > lenSecond) return true;
            if (lenFirst < lenSecond) return false;
        } else {
            if (lenFirst < lenSecond) return true;
            if (lenFirst > lenSecond) return false;
        }

        return compareDigits(first, second, isNegative);
    }
    // Funkcje pomocnicze:
    CharNode* skipLeadingZeros(CharNode* node) {
        if (node == nullptr || node->data == '\t') return node;
        if (node->data != '0') return node;
        return skipLeadingZeros(node->next);
    }
    bool compareDigits(CharNode* first, CharNode* second, bool isNegative) {
        // Warunki kończące
        if ((first == nullptr || first->data == '\t') &&
            (second == nullptr || second->data == '\t')) {
            return false;
            }

        if (isNegative) {
            if (first->data > second->data) return true;
            if (first->data < second->data) return false;
        } else {
            if (first->data < second->data) return true;
            if (first->data > second->data) return false;
        }
        return compareDigits(first->next, second->next, isNegative);
    }
    // "="
    void popABBinaryEqual() {
        if (isEmpty() || top->lower == nullptr) {
            return;
        }
        CharNode* listA = pop();
        CharNode* listB = pop();

        bool aNegative = isNegative(listA);
        bool bNegative = isNegative(listB);

        if (aNegative != bNegative) {
            if (aNegative==true) {removeSign(listA);}
            if (bNegative==true) {removeSign(listB);}
            if (containsOnlyZeros(listA) && containsOnlyZeros(listB)) {
                push(createResultList('1'));
            } else {
                push(createResultList('0'));
            }
            deleteList(listA);
            deleteList(listB);
            return;
        }

        if (aNegative) {
            removeSign(listA);
            removeSign(listB);
        }

        if (containsOnlyZeros(listA) && containsOnlyZeros(listB)) {
            push(createResultList('1'));
            deleteList(listA);
            deleteList(listB);
            return;
        }
        CharNode* reversedA = reverseList(listA);
        CharNode* reversedB = reverseList(listB);

        bool equal = compareNormalizedNumbers(reversedA, reversedB);

        push(createResultList(equal ? '1' : '0'));
        deleteList(listA);
        deleteList(listB);
        deleteList(reversedA);
        deleteList(reversedB);
    }
    // Funkcje pomocnicze:
    bool isNegative(CharNode* list) {
        CharNode* last = getLastBeforeSpace(list);
        return last != nullptr && last->data == '-';
    }
    void removeSign(CharNode*& list) {
        CharNode* last = getLastBeforeSpace(list);
        if (last != nullptr && last->data == '-') {
            CharNode* prev = getPreviousToLast(list);
            if (prev != nullptr) {
                prev->next = last->next;
            } else {
                list = last->next;
            }
            delete last;
        }
    }
    CharNode* getLastBeforeSpace(CharNode* node, CharNode* prev = nullptr) {
        if (node == nullptr || node->data == '\t') return prev;
        return getLastBeforeSpace(node->next, node);
    }
    CharNode* getPreviousToLast(CharNode* node, CharNode* prev = nullptr, CharNode* prevPrev = nullptr) {
        if (node == nullptr || node->data == '\t') return prevPrev;
        return getPreviousToLast(node->next, node, prev);
    }
    bool containsOnlyZeros(CharNode* node) {
        if (node == nullptr || node->data == '\t') return true;
        if (node->data != '0') return false;
        return containsOnlyZeros(node->next);
    }
    CharNode* reverseHelper(CharNode* node, CharNode* reversed) {
        if (node == nullptr || node->data == '\t') return reversed;
        CharNode* newNode = new CharNode{node->data, reversed};
        return reverseHelper(node->next, newNode);
    }
    CharNode* appendSpaceAtEnd(CharNode* head) {
        if (head == nullptr) return new CharNode{'\t', nullptr};
        head->next = appendSpaceAtEnd(head->next);
        return head;
    }
    CharNode* reverseList(CharNode* original) {
        CharNode* reversed = reverseHelper(original, nullptr);
        return appendSpaceAtEnd(reversed);
    }
    bool compareNormalizedNumbersRecursive(CharNode* a, CharNode* b) {
        if ((a == nullptr || a->data == '\t') && (b == nullptr || b->data == '\t')) return true;
        if ((a == nullptr || a->data == '\t') || (b == nullptr || b->data == '\t')) return false;
        if (a->data != b->data) return false;
        return compareNormalizedNumbersRecursive(a->next, b->next);
    }
    bool compareNormalizedNumbers(CharNode* a, CharNode* b) {
        a = skipLeadingZeros(a);
        b = skipLeadingZeros(b);
        return compareNormalizedNumbersRecursive(a, b);
    }
    CharNode* createResultList(char value) {
        CharNode* result = new CharNode{value, nullptr};
        result->next = new CharNode{'\t', nullptr};
        return result;
    }
    // "!"
    void negateLogically() {
        if (isEmpty()) {
            return;
        }

        CharNode* topList = top->data;
        bool shouldBeOne = false;

        if (topList == nullptr) {
            shouldBeOne = true; // Pusta lista -> '1'
        }
        else if (topList->data == '\t' && topList->next == nullptr) {
            shouldBeOne = true; // Tylko tab '\t' -> '1'
        }
        else if (topList->data == '0' && (topList->next == nullptr ||
                 (topList->next->data == '\t' && topList->next->next == nullptr))) {
            shouldBeOne = true; // Tylko '0'  -> '1'
                 }

        deleteList(top->data);

        CharNode* result = new CharNode{shouldBeOne ? '1' : '0', nullptr};
        result->next = new CharNode{'\t', nullptr};

        top->data = result;
    }
    //"~"
    void pushInstructionPointer(int instructionPointer) {
        CharNode* numberList = convertNumberToList(instructionPointer);
        push(numberList);
    }
    CharNode* convertNumberToList(int value, CharNode* tail = nullptr) {
        if (value == 0) {
            if (tail == nullptr) {
                return new CharNode{'0', new CharNode{'\t', nullptr}};
            }
            tail->next = new CharNode{'\t', nullptr};
            return nullptr;
        }

        int digit = value % 10;
        CharNode* newNode = new CharNode{static_cast<char>(digit + '0'), nullptr};

        if (tail != nullptr) {
            tail->next = newNode;
        }

        convertNumberToList(value / 10, newNode);
        return (tail == nullptr) ? newNode : tail;
    }
    //"?"
    bool containsOnlyZerosRecursive(CharNode* node) {
        if (node == nullptr || node->data == '\t') return true;
        if (node->data != '0') return false;
        return containsOnlyZerosRecursive(node->next);
    }
    void conditionalJump(int& instructionPointer) {
        if (isEmpty() || top->lower == nullptr) {
            return;
        }

        CharNode* T = pop();
        CharNode* W = pop();

        if (W == nullptr || (W->data == '\t' && W->next == nullptr)) {
            deleteList(W);
            deleteList(T);
            return;
        }

        if (!containsOnlyZerosRecursive(W)) {
            int targetPosition = convertListToNumber(T);
            instructionPointer = targetPosition - 1 > 0 ? targetPosition - 1 : 0;
        }

        deleteList(W);
        deleteList(T);
    }
    int convertListToNumber(CharNode* list, int position = 1, bool isNegative = false) {
        if (list == nullptr || list->data == '\t') {
            return 0;
        }

        if (position == 1 && list->data == '-') {
            return convertListToNumber(list->next, position, true);
        }
        if (list->data < '0' || list->data > '9') {
            return convertListToNumber(list->next, position, isNegative);
        }
        int digit = list->data - '0';
        int result = digit * position + convertListToNumber(list->next, position * 10, isNegative);

        return isNegative ? -result : result;
    }
    // "+"
    void addition() {
        if (isEmpty() || top->lower == nullptr) {
            return;
        }

        CharNode* listA = pop();
        CharNode* listB = pop();

        bool aNegative = isNegative(listA);
        bool bNegative = isNegative(listB);

        if (aNegative) removeSign(listA);
        if (bNegative) removeSign(listB);
        CharNode* result = nullptr;
        bool resultNegative = false;

        if (aNegative == bNegative) {
            result = addAbsoluteValues(listA, listB);
            resultNegative = aNegative;
        } else {
            int comparison = compareAbsoluteValues(listA, listB);
            listA = removeLeadingZeros(listA);
            listB = removeLeadingZeros(listB);
            if (comparison > 0) {
                result = subtractAbsoluteValues(listA, listB);
                resultNegative = aNegative;
            } else if (comparison < 0) {
                result = subtractAbsoluteValues(listB, listA);
                resultNegative = bNegative;
            } else {
                result = new CharNode{'0', nullptr};
                resultNegative = false;
            }
        }

        result = removeLeadingZeros(result);
        if (result == nullptr) {
            result = new CharNode{'0', nullptr};
        }

        // -/ '\t' na koniec
        CharNode* appendedSign = appendSign(result);

        if (resultNegative && !containsOnlyZeros(result)) {
            appendedSign->next = new CharNode{'-', new CharNode{'\t', nullptr}};
        } else {
            appendedSign->next = new CharNode{'\t', nullptr};
        }
        push(result);
        deleteList(listA);
        deleteList(listB);
    }
    CharNode* appendSign(CharNode* node) {
        if (node == nullptr || node->next == nullptr || node->next->next == nullptr) {
            return node;
        }
        return appendSign(node->next);
    }
    CharNode* addDigitsRecursive(CharNode* a, CharNode* b, int carry) {
        if ((a == nullptr || a->data == '\t') && (b == nullptr || b->data == '\t') && carry == 0) {
            return nullptr;
        }

        int digitA = (a != nullptr && a->data != '\t' && a->data != '-') ? (a->data - '0') : 0;
        int digitB = (b != nullptr && b->data != '\t' && b->data != '-') ? (b->data - '0') : 0;
        int sum = digitA + digitB + carry;
        int newCarry = sum / 10;
        int digit = sum % 10;
        CharNode* resultNode = new CharNode{static_cast<char>(digit + '0'), nullptr};
        CharNode* nextA = (a != nullptr && a->next != nullptr && a->data != '\t') ? a->next : nullptr;
        CharNode* nextB = (b != nullptr && b->next != nullptr && b->data != '\t') ? b->next : nullptr;

        resultNode->next = addDigitsRecursive(nextA, nextB, newCarry);
        return resultNode;
    }
    CharNode* addAbsoluteValues(CharNode* a, CharNode* b) {
        return addDigitsRecursive(a, b, 0);
    }
    CharNode* subtractDigitsRecursive(CharNode* greater, CharNode* smaller, int borrow) {
        if ((greater == nullptr || greater->data == '\t') && borrow == 0) {
            return nullptr;
        }
        int digitGreater = (greater != nullptr && greater->data != '\t' && greater->data != '-') ? (greater->data - '0') : 0;
        int digitSmaller = (smaller != nullptr && smaller->data != '\t' && smaller->data != '-') ? (smaller->data - '0') : 0;

        int difference = digitGreater - digitSmaller - borrow;

        if (difference < 0) {
            difference += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        CharNode* newNode = new CharNode{static_cast<char>(difference + '0'), nullptr};
        CharNode* nextGreater = (greater != nullptr && greater->next != nullptr && greater->data != '\t') ? greater->next : nullptr;
        CharNode* nextSmaller = (smaller != nullptr && smaller->next != nullptr && smaller->data != '\t') ? smaller->next : nullptr;

        newNode->next = subtractDigitsRecursive(nextGreater, nextSmaller, borrow);
        return newNode;
    }
    CharNode* subtractAbsoluteValues(CharNode* greater, CharNode* smaller) {
        return subtractDigitsRecursive(greater, smaller, 0);
    }
    CharNode* removeLeadingZerosRecursive(CharNode* current) {
        if (current == nullptr || current->data != '0') return current;

        CharNode* toDelete = current;
        current = current->next;
        delete toDelete;
        return removeLeadingZerosRecursive(current);
    }
    CharNode* restoreOriginalOrderRecursive(CharNode* current, CharNode* reversed) {
        if (current == nullptr) return reversed;
        CharNode* newNode = new CharNode{current->data, reversed};
        reversed = newNode;
        return restoreOriginalOrderRecursive(current->next, reversed);
    }
    CharNode* findLastNode(CharNode* current) {
        if (current == nullptr || current->next == nullptr) {
            return current;
        }
        return findLastNode(current->next);
    }
    CharNode* removeLeadingZeros(CharNode* list) {
        if (list == nullptr) {
            return nullptr;
        }
        CharNode* reversed = reverseListRecursive(list, nullptr);
        CharNode* nonZero = reversed;
        if (nonZero->data == '\t') {
            CharNode* temp = nonZero;
            nonZero = nonZero->next;
            delete temp;
        }
        nonZero = removeLeadingZerosRecursive(nonZero);
        if (nonZero == nullptr) {
            return new CharNode{'0', new CharNode{'\t', nullptr}};
        }
        CharNode* finalReversed = restoreOriginalOrderRecursive(nonZero, nullptr);
        CharNode* tail = findLastNode(finalReversed);
        tail->next = new CharNode{'\t', nullptr};
        return finalReversed;
    }
    int compareNodesRecursive(CharNode* a, CharNode* b) {
        if (a == nullptr || a->data == '\t' || a->data == '-') return 0;
        if (b == nullptr || b->data == '\t' || b->data == '-') return 0;

        if (a->data != b->data) {
            return a->data - b->data;
        }
        return compareNodesRecursive(a->next, b->next);
    }
    int compareAbsoluteValues(CharNode* a, CharNode* b) {
        CharNode* cloneA = cloneList(a);
        CharNode* cloneB = cloneList(b);

        cloneA = removeLeadingZeros(cloneA);
        cloneB = removeLeadingZeros(cloneB);

        int lenA = listLengthWithoutSpace(cloneA);
        int lenB = listLengthWithoutSpace(cloneB);

        if (lenA != lenB) {
            int result = lenA - lenB;
            deleteList(cloneA);
            deleteList(cloneB);
            return result;
        }
        CharNode* revA = reverseListWithoutSpace(cloneA);
        CharNode* revB = reverseListWithoutSpace(cloneB);
        int result = compareNodesRecursive(revA, revB);
        deleteList(cloneA);
        deleteList(cloneB);
        deleteList(revA);
        deleteList(revB);

        return result;
    }
    CharNode* cloneListRecursive(CharNode* origCurrent) {
        if (origCurrent == nullptr) return nullptr;
        CharNode* newNode = new CharNode{origCurrent->data, nullptr};
        newNode->next = cloneListRecursive(origCurrent->next);
        return newNode;
    }
    CharNode* cloneList(CharNode* original) {
        if (original == nullptr) return nullptr;
        return cloneListRecursive(original);
    }
    int listLengthWithoutSpaceRecursive(CharNode* current) {
        if (current == nullptr || current->data == '\t') return 0;
        int length = (current->data != '-') ? 1 : 0;
        return length + listLengthWithoutSpaceRecursive(current->next);
    }
    int listLengthWithoutSpace(CharNode* list) {
        return listLengthWithoutSpaceRecursive(list);
    }
    CharNode* reverseListRecursive(CharNode* current, CharNode* reversed) {
        if (current == nullptr || current->data == '\t') return reversed;
        if (current->data != '-') {
            CharNode* newNode = new CharNode{current->data, reversed};
            reversed = newNode;
        }
        return reverseListRecursive(current->next, reversed);
    }
    CharNode* reverseListWithoutSpace(CharNode* original) {
        return reverseListRecursive(original, nullptr);
    }
    void deleteList(CharNode* node) {
        if (node == nullptr) {
            return;
        }
        CharNode* next = node->next;
        delete node;
        deleteList(next);
    }
    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }
};
void execInstruction(char s, Stack& stack, int& instructionPointer) {
    switch (s) {
        case '\'':
            stack.pushEmptyList();
        break;
        case ',':
            stack.pop();
        break;
        case ':':
            stack.duplicateTop();
        break;
        case ';':
            stack.swapTop();
        break;
        case '@':
            stack.pushA();
        break;
        case '&':
            stack.printStack();
        break;
        case '.':
            stack.appendCharTop();
        break;
        case '>':
            stack.printFirstAndPop();
        break;
        case '-':
            stack.negate();
        break;
        case '^':
            stack.absolute();
        break;
        case '$':
            stack.separate();
        break;
        case '#':
            stack.popAndAddChar();
        break;
        case '[':
            stack.popAndAddNumeric();
        break;
        case ']':
            stack.popAndAddAscii();
        break;
        case '<':
            stack.popABBinaryLess();
        break;
        case '=':
            stack.popABBinaryEqual();
        break;
        case '!':
            stack.negateLogically();
        break;
        case '~':
            stack.pushInstructionPointer(instructionPointer);
        break;
        case '?':
            stack.conditionalJump(instructionPointer);
        break;
        case '+':
            stack.addition();
        break;
        default:
            stack.addChar(s);
    }
}
int main() {
    char program [20000];
    Stack stack;
    int instructionPointer=0;
    cin>>program;
    char now = program[instructionPointer];
    while (now != '\0') {
        now = program[instructionPointer];
        execInstruction(now,stack, instructionPointer);
        instructionPointer++;
    }
}