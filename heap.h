/******************************************************
  Heap.h -- Declarations for Basic Heap-of-Pair-of-Ints Class

  Stores pairs <element,priority> of ints.
  Supports O(log n) insertion, O(1) peeking at min priority and element 
  with min priority, and O(log n) extraction of element with min priority.
*******************************************************/
#include <math.h>

using namespace std;

class Heap{
friend class Heap; //having access to private members makes it easier to merge two heaps

public:
   // Constructors and Destructor

   // New empty Heap with default capacity.
   Heap();  

   // New empty Heap with capacity c.
   Heap(int c); 

   // New Heap with size s, consisting of pairs <Pi,Ei> where, 
   // for 0 <= i < s, Pi is Priorities[i] and Ei is value Elements[i].  
   // Capacity is s + c, where c is the "spare capacity" argument.
   // Requires: Priorities and Elements are of size at least s. 
   Heap( const int * Priorities, const int * Elements, int s, int c); 

   // New Heap with combined contents the two Heap arguments. 
   // Size of the new Heap is the sum of the sizes of argument Heaps.
   // Capacity of the new Heap is its size plus the "spare capacity" c.
   Heap( const Heap & Heap1, const Heap & Heap2, int c ); 

   // Destructor.
   ~Heap(); 

   // Accessors
   bool empty() const {return hSize == 0;}; // True iff Heap is empty.
   int size() const { return hSize ;} ; // Current size of Heap.
   int capacity() const { return hCapacity ;} ; // Current capacity.
   int peekMin() const { return A[0].element ;} // Peek at minimum priority element.
   int peekMinPriority() const { return A[0].priority ;} // Peek at minimum priority.

   // Modifiers
   void insert( int element, int priority ); // Insert the pair <element,priority>.
   int extractMin(); // Remove and return the highest (minimum) priority element.

   //testing
   void printHeap();

private:
   class Pair{
      public:
        int element ;
        int priority ;
   };

   Pair* A ; // Array containing heap contents.
   int hCapacity ; // Max number of elements (= size of A).
   int hSize ; // Current number of elements.

   // Repairs ordering invariant after adding leaf at A[i].
   void trickleUp(int i);

   // Repairs ordering invariant for sub-tree rooted at index i,
   //   when A[i] may be have larger priority than one of its children,
   //   but the subtrees of its children are heaps.
   void trickleDown(int i);

   // Establishes ordering invariant for entire array contents.
   void heapify(); //(Same as "make_heap" in lectures.)

   // Useful for implementing trickle up and down
   void swap(int i,int j);
};

Heap::Heap(){
   hCapacity = 10 ;
   A = new Pair[hCapacity];
   hSize = 0 ; 
}

Heap::Heap(int c){ // New empty Heap with capacity c.
	hCapacity = c;
	A = new Pair[hCapacity];
	hSize = 0;
}

// New Heap with capacity c+s, with s elements, consisting of pairs <Pi,Vi> where 
//  Pi is Priorities[i], Ei is value Elements[i], for 0 <= i < s.
Heap::Heap( const int * Priorities, const int * Elements, int s, int c){ 
	A = new Pair[s + c];
	hCapacity = s + c;
	
	for (int i = 0; i < s; i++) { //construct each element and insert into heap
		insert(Elements[i], Priorities[i]);
	}
}

// New Heap with combined contents and of the two given heaps.
Heap::Heap(const Heap & Heap1, const Heap & Heap2, int c) {
	hSize = Heap1.hSize + Heap2.hSize;
	hCapacity = hSize + c;
	A = new Pair[hCapacity];
	
	std::copy(Heap1.A, Heap1.A + Heap1.hSize, A); //copy contents of Heap1 to heap being constructed
	std::copy(Heap2.A, Heap2.A + Heap2.hSize, A + Heap1.hSize); //copy contents of Heap2 after the contents of Heap1 in the heap being constructed
	heapify(); //restore heap property of A
}

// Destructor
Heap::~Heap(){
   delete[] A;
}

// Modifiers

void Heap::insert(int element, int priority){
   A[hSize].element = element;
   A[hSize].priority = priority;
   trickleUp(hSize);
   hSize++;
}

// Repairs the heap ordering invariant after adding a new element.
// Initial call should be trickleUp(hSize-1).
void Heap::trickleUp(int i){
	//terminate at root or when heap property satisfied
	if (i == 0 || A[i].priority >= A[(i-1) / 2].priority) {
		return;
	}
	swap(i, (i-1) / 2); //swap current item and parent
	return trickleUp((i-1) / 2);
}

void Heap::swap(int i, int j){
   Pair temp = A[i];
   A[i] = A[j];
   A[j] = temp ;
}

// Removes and returns the element with highest priority.
// (That is, the element associated with the minimum priority value.)
int Heap::extractMin(){
	int min = A[0].element;
	A[0] = A[hSize - 1];
	hSize--;
	trickleDown(0);
	return min;
}

// Repairs the heap ordering invariant after replacing the root.
// (extractMin() calls trickleDown(0)).
// (trickleDown(i) performs the repair on the subtree rooted a A[i].)
// (heapify() calls trickleDown(i) for i from (hSize/2)-1 down to 0.)
void Heap::trickleDown(int i){
	int left = (2 * i) + 1; //left child of i
	int right = (2 * i) + 2; //right child of i
	
	//we are at leaf node, nothing to do
	if (left > hSize-1 && right > hSize-1) {
		return;
    }
	//left child is leaf and no right child, check if i is smaller and swap accordingly
	else if (right > hSize - 1 && A[i].priority > A[left].priority) {
		swap(i, left);
		return;
	}
	//some internal node, verify it has two children
	else if (left < hSize && right < hSize){
		//i has smaller/equal priority than children, no swap required
		if (A[i].priority <= A[left].priority && A[i].priority <= A[right].priority) {
			return;
		}
		//right child is smallest, swap accordingly
		if (A[left].priority > A[right].priority) {
			swap(i, right);
			trickleDown(right);
		}
		//left child is smallest, swap accordingly
		else {
			swap(i, left);
			trickleDown(left);
		}
	}
	return;
}

// Turns A[0] .. A[size-1] into a heap.
void Heap::heapify(){
   for( int i = (hSize/2)-1; i>=0 ; i-- ) trickleDown(i);  
}
 
//print contents of heap for debugging
void Heap::printHeap() {
	for (int i = 0; i < hSize; i++) {
		std::cout << "pos:" << i << " key:" << A[i].priority << " elem:" << A[i].element << std::endl;
	}
}
