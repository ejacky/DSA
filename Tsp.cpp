#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
using namespace std;

/******************************************************************************************
* ���ԡ���ʾ��С����
******************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <typeinfo.h>

#if defined(DSA_DEBUG) //���뿪�أ����Ƶ������
#else
#define DSA(x) //�����
#endif


/******************************************************************************************
* �б������Ƚṹ�ڵĽڵ��У����Դ�Ż������ͻ�������
* ���ձ���Լ��������Ч�ʵĿ��ǣ����ں�һ���ͨ��ֻ��������ָ��
* ��ˣ�������ṹ����֮ǰ����Ҫ�����ͷ���Щ��Ա������ռ�Ŀռ�
* �˴�������C++��ƫ�ػ����������������������������Ӧ����
******************************************************************************************/

template <typename T> struct Cleaner {
	static void clean(T x) { //�൱�ڵݹ��
#ifdef _DEBUG
		static int n = 0;
		if (7 > strlen(typeid (T).name())) { //��������һ�ź��ԣ�ֻ�����������
			printf("\t<%s>[%d]=", typeid (T).name(), ++n);
			print(x);
			printf(" purged\n");
		}
#endif
	}
};

template <typename T> struct Cleaner<T*> {
	static void clean(T* x) {
		if (x) { delete x; } //������а���ָ�룬�ݹ��ͷ�
#ifdef _DEBUG
		static int n = 0;
		printf("\t<%s>[%d] released\n", typeid (T*).name(), ++n);
#endif
	}
};

template <typename T> void release(T x) { Cleaner<T>::clean(x); }

/*****************
* Class Vector
******************/
//#define INT_MAX       2147483647
//#define _CRT_SECURE_NO_DEPRECATE
//
//#include <typeinfo.h>
//#include <stdio.h>
//#pragma once

typedef int Rank; //��
#define DEFAULT_CAPACITY  3 //Ĭ�ϵĳ�ʼ������ʵ��Ӧ���п�����Ϊ����

template <typename T> class Vector { //����ģ����
protected:
	Rank _size; int _capacity;  T* _elem; //��ģ��������������
	void copyFrom(T const* A, Rank lo, Rank hi); //������������A[lo, hi)
	void expand(); //�ռ䲻��ʱ����
	void shrink(); //װ�����ӹ�Сʱѹ��
	//bool bubble(Rank lo, Rank hi); //ɨ�轻��
	//void bubbleSort(Rank lo, Rank hi); //���������㷨
	//Rank max(Rank lo, Rank hi); //ѡȡ���Ԫ��
	//void selectionSort(Rank lo, Rank hi); //ѡ�������㷨
	//void merge(Rank lo, Rank mi, Rank hi); //�鲢�㷨
	//void mergeSort(Rank lo, Rank hi); //�鲢�����㷨
	//Rank partition(Rank lo, Rank hi); //��㹹���㷨
	//void quickSort(Rank lo, Rank hi); //���������㷨
	//void heapSort(Rank lo, Rank hi); //�������Ժ�����ȫ�ѽ��⣩
public:
	// ���캯��
	Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0) //����Ϊc����ģΪs������Ԫ�س�ʼΪv
	{
		_elem = new T[_capacity = c]; for (_size = 0; _size < s; _elem[_size++] = v);
	} //s<=c
	Vector(T const* A, Rank n) { copyFrom(A, 0, n); } //�������帴��
	Vector(T const* A, Rank lo, Rank hi) { copyFrom(A, lo, hi); } //����
	Vector(Vector<T> const& V) { copyFrom(V._elem, 0, V._size); } //�������帴��
	Vector(Vector<T> const& V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); } //����
	//// ��������
	~Vector() { delete[] _elem; } //�ͷ��ڲ��ռ�
	//// ֻ�����ʽӿ�
	Rank size() const { return _size; } //��ģ
	bool empty() const { return !_size; } //�п�
	//int disordered() const; //�ж������Ƿ�������
	//Rank find(T const& e) const { return find(e, 0, _size); } //���������������
	//Rank find(T const& e, Rank lo, Rank hi) const; //���������������
	//Rank search(T const& e) const //���������������
	//{
	//	return (0 >= _size) ? -1 : search(e, 0, _size);
	//}
	//Rank search(T const& e, Rank lo, Rank hi) const; //���������������
	//// ��д���ʽӿ�
	T& operator[] (Rank r) const; //�����±������������������������ʽ���ø�Ԫ��
	Vector<T> & operator= (Vector<T> const&); //���ظ�ֵ���������Ա�ֱ�ӿ�¡����
	T remove(Rank r); //ɾ����Ϊr��Ԫ��
	int remove(Rank lo, Rank hi); //ɾ����������[lo, hi)֮�ڵ�Ԫ��
	Rank insert(Rank r, T const& e); //����Ԫ��
	Rank insert(T const& e) { return insert(_size, e); } //Ĭ����ΪĩԪ�ز���
	//void sort(Rank lo, Rank hi); //��[lo, hi)����
	//void sort() { sort(0, _size); } //��������
	//void unsort(Rank lo, Rank hi); //��[lo, hi)����
	//void unsort() { unsort(0, _size); } //��������
	//int deduplicate(); //����ȥ��
	//int uniquify(); //����ȥ��
	//// ����
	//void traverse(void(*) (T&)); //������ʹ�ú���ָ�룬ֻ����ֲ����޸ģ�
	//template <typename VST> void traverse(VST&); //������ʹ�ú������󣬿�ȫ�����޸ģ�
}; //Vector

template <typename T> T& Vector<T>::operator[] (Rank r) const //�����±������
{
	return _elem[r];
} // assert: 0 <= r < _size

template <typename T> Vector<T>& Vector<T>::operator= (Vector<T> const& V) { //����
	if (_elem) delete[] _elem; //�ͷ�ԭ������
	copyFrom(V._elem, 0, V.size()); //���帴��
	return *this; //���ص�ǰ��������ã��Ա���ʽ��ֵ
}

template <typename T> //��e��Ϊ��ΪrԪ�ز���
Rank Vector<T>::insert(Rank r, T const& e) { //assert: 0 <= r <= size
	expand(); //���б�Ҫ������
	for (int i = _size; i > r; i--) _elem[i] = _elem[i - 1]; //�Ժ���ǰ�����Ԫ��˳�κ���һ����Ԫ
	_elem[r] = e; _size++; //������Ԫ�ز���������
	return r; //������
}

template <typename T> void Vector<T>::expand() { //�����ռ䲻��ʱ����
	if (_size < _capacity) return; //��δ��Աʱ����������
	if (_capacity < DEFAULT_CAPACITY) _capacity = DEFAULT_CAPACITY; //��������С����
	T* oldElem = _elem;  _elem = new T[_capacity <<= 1]; //�����ӱ�
	for (int i = 0; i < _size; i++)
		_elem[i] = oldElem[i]; //����ԭ�������ݣ�TΪ�������ͣ��������ظ�ֵ������'='��
	/*DSA*/ //printf("\n_ELEM [%x]*%d/%d expanded and shift to [%x]*%d/%d\n", oldElem, _size, _capacity/2, _elem, _size, _capacity);
	delete[] oldElem; //�ͷ�ԭ�ռ�
}

template <typename T> //Ԫ������
void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi) { //����������A[lo, hi)Ϊ������������
	_elem = new T[_capacity = 2 * (hi - lo)]; _size = 0; //����ռ䣬��ģ����
	while (lo < hi) //A[lo, hi)�ڵ�Ԫ����һ
		_elem[_size++] = A[lo++]; //������_elem[0, hi - lo)
}

template <typename T> int Vector<T>::remove(Rank lo, Rank hi) { //ɾ������[lo, hi)
	if (lo == hi) return 0; //����Ч�ʿ��ǣ����������˻����������remove(0, 0)
	while (hi < _size) _elem[lo++] = _elem[hi++]; //[hi, _size)˳��ǰ��hi - lo����Ԫ
	_size = lo; //���¹�ģ��ֱ�Ӷ���β��[lo, _size = hi)����
	shrink(); //���б�Ҫ��������
	return hi - lo; //���ر�ɾ��Ԫ�ص���Ŀ
}

template <typename T> T Vector<T>::remove(Rank r) { //ɾ����������Ϊr��Ԫ�أ�0 <= r < size
	T e = _elem[r]; //���ݱ�ɾ��Ԫ��
	remove(r, r + 1); //��������ɾ���㷨����Ч�ڶ�����[r, r + 1)��ɾ��
	return e; //���ر�ɾ��Ԫ��
}

template <typename T> void Vector<T>::shrink() { //װ�����ӹ�Сʱѹ��������ռ�ռ�
	if (_capacity < DEFAULT_CAPACITY << 1) return; //����������DEFAULT_CAPACITY����
	if (_size << 2 > _capacity) return; //��25%Ϊ��
	T* oldElem = _elem;  _elem = new T[_capacity >>= 1]; //��������
	for (int i = 0; i < _size; i++) _elem[i] = oldElem[i]; //����ԭ��������
	delete[] oldElem; //�ͷ�ԭ�ռ�
}


/*****************
* Stack @ Vector
******************/
template <typename T> class Stack : public Vector<T> { //����������/ĩ����Ϊջ��/��
public: //size()��empty()�Լ��������Žӿڣ�����ֱ������
	void push(T const& e) { insert(size(), e); } //��ջ����Ч�ڽ���Ԫ����Ϊ������ĩԪ�ز���
	T pop() { return remove(size() - 1); } //��ջ����Ч��ɾ��������ĩԪ��
	T& top() { return (*this)[size() - 1]; } //ȡ����ֱ�ӷ���������ĩԪ��
};

/*****************
* Class Graph
******************/
#pragma once

typedef enum { UNDISCOVERED, DISCOVERED, VISITED } VStatus; //����״̬
typedef enum { UNDETERMINED, TREE, CROSS, FORWARD, BACKWARD } EType; //���ڱ�����������������

template <typename Tv, typename Te> //�������͡�������
class Graph { //ͼGraphģ����
private:
	void reset() { //���ж��㡢�ߵĸ�����Ϣ��λ
		for (int i = 0; i < n; i++) { //���ж����
			status(i) = UNDISCOVERED; dTime(i) = fTime(i) = -1; //״̬��ʱ���ǩ
			parent(i) = -1; priority(i) = INT_MAX; //���ڱ������еģ����ڵ㣬���ȼ���
			for (int j = 0; j < n; j++) //���бߵ�
			if (exists(i, j)) type(i, j) = UNDETERMINED; //����
		}
	}
	//void BFS(int, int&); //����ͨ�򣩹�����������㷨
	//void DFS(int, int&); //����ͨ��������������㷨
	//void BCC(int, int&, Stack<int>&); //����ͨ�򣩻���DFS��˫��ͨ�����ֽ��㷨
	bool TSort(int, int&, Stack<Tv>*); //����ͨ�򣩻���DFS�����������㷨
	//template <typename PU> void PFS(int, PU); //����ͨ�����ȼ��������
public:
	// ����
	int n; //��������
	virtual int insert(Tv const&) = 0; //���붥�㣬���ر��
	virtual Tv remove(int) = 0; //ɾ�����㼰������ߣ����ظö�����Ϣ
	virtual Tv& vertex(int) = 0; //����v�����ݣ��ö����ȷ���ڣ�
	virtual int inDegree(int) = 0; //����v����ȣ��ö����ȷ���ڣ�
	virtual int outDegree(int) = 0; //����v�ĳ��ȣ��ö����ȷ���ڣ�
	virtual int firstNbr(int) = 0; //����v���׸��ڽӶ���
	virtual int nextNbr(int, int) = 0; //����v�ģ�����ڶ���j�ģ���һ�ڽӶ���
	virtual VStatus& status(int) = 0; //����v��״̬
	virtual int& dTime(int) = 0; //����v��ʱ���ǩdTime
	virtual int& fTime(int) = 0; //����v��ʱ���ǩfTime
	virtual int& parent(int) = 0; //����v�ڱ������еĸ���
	virtual int& priority(int) = 0; //����v�ڱ������е����ȼ���
	// �ߣ�����Լ��������߾�ͳһת��Ϊ�������һ������ߣ��Ӷ�������ͼ��������ͼ������
	int e; //������
	virtual bool exists(int, int) = 0; //��(v, u)�Ƿ����
	virtual void insert(Te const&, int, int, int) = 0; //�ڶ���v��u֮�����Ȩ��Ϊw�ı�e
	virtual Te remove(int, int) = 0; //ɾ������v��u֮��ı�e�����ظñ���Ϣ
	virtual EType & type(int, int) = 0; //��(v, u)������
	virtual Te& edge(int, int) = 0; //��(v, u)�����ݣ��ñߵ�ȷ���ڣ�
	virtual int& weight(int, int) = 0; //��(v, u)��Ȩ��
	// �㷨
	void bfs(int); //������������㷨
	void dfs(int); //������������㷨
	void bcc(int); //����DFS��˫��ͨ�����ֽ��㷨
	Stack<Tv>* tSort(int); //����DFS�����������㷨
	void prim(int); //��С֧����Prim�㷨
	void dijkstra(int); //���·��Dijkstra�㷨
	template <typename PU> void pfs(int, PU); //���ȼ��������
};

/*****************
* grap tsort
******************/
#pragma once

template <typename Tv, typename Te> //����DFS�����������㷨
Stack<Tv>* Graph<Tv, Te>::tSort(int s) { //assert: 0 <= s < n
	reset(); int clock = 0; int v = s;
	Stack<Tv>* S = new Stack<Tv>; //��ջ��¼���򶥵�
	do {
		if (UNDISCOVERED == status(v))
		if (!TSort(v, clock, S)) { //clock���Ǳ���
			/*DSA*/print(S);
			while (!S->empty()) //��һ��ͨ���༴��ͼ����DAG
				S->pop(); break; //�򲻱ؼ������㣬��ֱ�ӷ���
		}
	} while (s != (v = (++v % n)));
	return S; //������ΪDAG����S�ڸ������Զ�������򣻷��򣨲������������򣩣�S��
}

template <typename Tv, typename Te> //����DFS�����������㷨�����ˣ�
bool Graph<Tv, Te>::TSort(int v, int& clock, Stack<Tv>* S) { //assert: 0 <= v < n
	dTime(v) = ++clock; status(v) = DISCOVERED; //���ֶ���v
	for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u)) //ö��v�������ھ�u
		switch (status(u)) { //����u��״̬�ֱ���
		case UNDISCOVERED:
			parent(u) = v; type(v, u) = TREE;
			if (!TSort(u, clock, S)) //�Ӷ���u��������������
				return false; //��u��������������������ȫͼ�����ˣ����ʷ��ز�����
			break;
		case DISCOVERED:
			type(v, u) = BACKWARD; //һ�����ֺ���ߣ���DAG������
			return false; //�������룬�ʷ��ز�����
		default: //VISITED (digraphs only)
			type(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS;
			break;
	}
	status(v) = VISITED; S->push(vertex(v)); //���㱻���ΪVISITEDʱ���漴��ջ
	return true; //v������������������
}


/*****************
* Class GraphMatrix
******************/

template <typename Tv> struct Vertex { //�������Ϊ���������δ�ϸ��װ��
	Tv data; int inDegree, outDegree; VStatus status; //���ݡ����������״̬
	int dTime, fTime; //ʱ���ǩ
	int parent; int priority; //�ڱ������еĸ��ڵ㡢���ȼ���
	Vertex(Tv const& d = (Tv)0) : //�����¶���
		data(d), inDegree(0), outDegree(0), status(UNDISCOVERED),
		dTime(-1), fTime(-1), parent(-1), priority(INT_MAX) {} //�ݲ�����Ȩ�����
};

template <typename Te> struct Edge { //�߶���Ϊ���������δ�ϸ��װ��
	Te data; int weight; EType type; //���ݡ�Ȩ�ء�����
	Edge(Te const& d, int w) : data(d), weight(w), type(UNDETERMINED) {} //����
};


template <typename Tv, typename Te> //�������͡�������
class GraphMatrix : public Graph<Tv, Te> { //�������������ڽӾ�����ʽʵ�ֵ�ͼ
private:
	Vector< Vertex< Tv > > V; //���㼯��������
	Vector< Vector< Edge< Te > * > > E; //�߼����ڽӾ���
public:
	GraphMatrix() { n = e = 0; } //����
	~GraphMatrix() { //����
		for (int j = 0; j < n; j++) //���ж�̬������
		for (int k = 0; k < n; k++) //�߼�¼
			delete E[j][k]; //�������
	}
	// ����Ļ�����������ѯ��i�����㣨0 <= i < n��
	virtual Tv& vertex(int i) { return V[i].data; } //����
	virtual int inDegree(int i) { return V[i].inDegree; } //���
	virtual int outDegree(int i) { return V[i].outDegree; } //����
	virtual int firstNbr(int i) { return nextNbr(i, n); } //�׸��ڽӶ���
	virtual int nextNbr(int i, int j) //����ڶ���j����һ�ڽӶ��㣨�����ڽӱ�����Ч�ʣ�
	{
		while ((-1 < j) && (!exists(i, --j))); return j;
	} //����������̽
	virtual VStatus& status(int i) { return V[i].status; } //״̬
	virtual int& dTime(int i) { return V[i].dTime; } //ʱ���ǩdTime
	virtual int& fTime(int i) { return V[i].fTime; } //ʱ���ǩfTime
	virtual int& parent(int i) { return V[i].parent; } //�ڱ������еĸ���
	virtual int& priority(int i) { return V[i].priority; } //�ڱ������е����ȼ���
	// ����Ķ�̬����
	virtual int insert(Tv const& vertex) { //���붥�㣬���ر��
		for (int j = 0; j < n; j++) E[j].insert(NULL); n++; //������Ԥ��һ��Ǳ�ڵĹ�����
		E.insert(Vector<Edge<Te>*>(n, n, (Edge<Te>*) NULL)); //�����¶����Ӧ�ı�����
		return V.insert(Vertex<Tv>(vertex)); //������������һ������
	}
	virtual Tv remove(int i) { //ɾ����i�����㼰������ߣ�0 <= i < n��
		for (int j = 0; j < n; j++) //���г���
		if (exists(i, j)) { delete E[i][j]; V[j].inDegree--; } //����ɾ��
		E.remove(i); n--; //ɾ����i��
		Tv vBak = vertex(i); V.remove(i); //ɾ������i
		for (int j = 0; j < n; j++) //�������
		if (Edge<Te> * e = E[j].remove(i)) { delete e; V[j].outDegree--; } //����ɾ��
		return vBak; //���ر�ɾ���������Ϣ
	}
	// �ߵ�ȷ�ϲ���
	virtual bool exists(int i, int j) //��(i, j)�Ƿ����
	{
		return (0 <= i) && (i < n) && (0 <= j) && (j < n) && E[i][j] != NULL;
	}
	// �ߵĻ�����������ѯ����i��j֮������ߣ�0 <= i, j < n��exists(i, j)��
	virtual EType & type(int i, int j) { return E[i][j]->type; } //��(i, j)������
	virtual Te& edge(int i, int j) { return E[i][j]->data; } //��(i, j)������
	virtual int& weight(int i, int j) { return E[i][j]->weight; } //��(i, j)��Ȩ��
	// �ߵĶ�̬����
	virtual void insert(Te const& edge, int w, int i, int j) { //����Ȩ��Ϊw�ı�e = (i, j)
		if (exists(i, j)) return; //ȷ���ñ��в�����
		E[i][j] = new Edge<Te>(edge, w); //�����±�
		e++; V[i].outDegree++; V[j].inDegree++; //���±߼������������Ķ���
	}
	virtual Te remove(int i, int j) { //ɾ������i��j֮������ߣ�exists(i, j)��
		Te eBak = edge(i, j); delete E[i][j]; E[i][j] = NULL; //���ݺ�ɾ���߼�¼
		e--; V[i].outDegree--; V[j].inDegree--; //���±߼������������Ķ���
		return eBak; //���ر�ɾ���ߵ���Ϣ
	}
};


/******************************************************************************************
* ����Ԫ�ء����ݽṹͨ������ӿ�
******************************************************************************************/
template <typename T> static void print(T* x) { x ? print(*x) : printf(" <NULL>"); }
template <typename T> static void print(T& x) { UniPrint::p(x); }
template <typename T> static void print(const T& x) { UniPrint::p(x); } //for Stack
static void print(char* x) { printf(" %s", x ? x : "<NULL>"); } //�ַ����ر���
static void print(const char* x) { printf(" %s", x ? x : "<NULL>"); } //�ַ����ر���

class UniPrint {
public:
	static void p(int);
	static void p(float);
	static void p(double);
	static void p(char);
	//static void p(HuffChar&); //Huffman�������ַ�
	static void p(VStatus); //ͼ�����״̬
	static void p(EType); //ͼ�ߵ�����


	template <typename Tv, typename Te> static void p(GraphMatrix<Tv, Te>&); //Graph
	template <typename T> static void p(T&); //�������б��֧��traverse()�������������Խṹ
	template <typename T> static void p(T* s) //����ָ��
	{
		s ? p(*s) : print("<NULL>");
	} //ͳһתΪ����
}; //UniPrint

/******************************************************************************************
* ��������
******************************************************************************************/
void UniPrint::p(int e) { printf(" %04d", e); }
void UniPrint::p(float e) { printf(" %4.1f", e); }
void UniPrint::p(double e) { printf(" %4.1f", e); }
void UniPrint::p(char e) { printf(" %c", (31 < e) && (e < 128) ? e : '$'); }
void UniPrint::p(VStatus e) {
	switch (e) {
	case UNDISCOVERED:   printf("U"); break;
	case DISCOVERED:     printf("D"); break;
	case VISITED:        printf("V"); break;
	default:             printf("X"); break;
	}
}
void UniPrint::p(EType e) {
	switch (e) {
	case UNDETERMINED:   printf("U"); break;
	case TREE:           printf("T"); break;
	case CROSS:          printf("C"); break;
	case BACKWARD:       printf("B"); break;
	case FORWARD:        printf("F"); break;
	default:             printf("X"); break;
	}
}

/******************************************************************************************
* ͼGraph
******************************************************************************************/
template <typename Tv, typename Te> //�������͡�������
void UniPrint::p(GraphMatrix<Tv, Te>& s) { //����
	int inD = 0; for (int i = 0; i < s.n; i++) inD += s.inDegree(i);
	int outD = 0; for (int i = 0; i < s.n; i++) outD += s.outDegree(i);
	printf("%s[%d]*(%d, %d):\n", typeid (s).name(), &s, s.n, s.e); //������Ϣ
	// ������
	print(s.n); printf(" "); print(inD); printf("|");
	for (int i = 0; i < s.n; i++) { print(s.vertex(i)); printf("["); print(s.status(i)); printf("] "); }
	printf("\n");
	// �����У�����
	print(outD); printf(" "); print(s.e); printf("|");
	for (int i = 0; i < s.n; i++) { print(s.inDegree(i)); printf(" "); }
	printf("| dTime fTime Parent Weight\n");
	// ˮƽ�ָ���
	printf("-----------+"); for (int i = 0; i < s.n; i++) printf("------");
	printf("+----------------------------\n");
	// �������������
	for (int i = 0; i < s.n; i++) {
		print(s.vertex(i)); printf("["); print(s.status(i)); printf("] "); print(s.outDegree(i)); printf("|");
		for (int j = 0; j < s.n; j++)
		if (s.exists(i, j)) { print(s.edge(i, j)); print(s.type(i, j)); }
		else printf("     .");
		printf("| "); print(s.dTime(i)); printf(" "); print(s.fTime(i));
		printf("     "); if (0 > s.parent(i)) print("^"); else print(s.vertex(s.parent(i)));
		printf("  "); if (INT_MAX > s.priority(i)) print(s.priority(i)); else print(" INF");
		printf("\n");
	}
	printf("\n");
}

/******************************************************************************************
* �������б��֧��traverse()�������������Խṹ
******************************************************************************************/
template <typename T> //Ԫ������
void UniPrint::p(T& s) { //����
	printf("%s[%d]*%d:\n", typeid (s).name(), &s, s.size()); //������Ϣ
	s.traverse(print); //ͨ��print()�����������Ԫ��
	printf("\n");
}



void importGraph(GraphMatrix<char, int> & g) {
	int n; scanf("%d\n", &n); /*DSA*/printf("%d vertices\n", n);
	for (int i = 0; i < n; i++) { //����v������
		char vertex; scanf("%c", &vertex);  /*DSA*/printf("Inserting vertex"); print(vertex); printf(" ...");
		g.insert(vertex);                  /*DSA*/printf("done\n"); print(g);
	}
	for (int i = 0; i < n; i++)
	for (int j = 0; j < n; j++) { //�����
		int edge; scanf("%d", &edge);
		if (0 > edge) continue;             /*DSA*/printf("Inserting edge (%d, %d) = ", i, j); print(edge); printf(" ...");
		g.insert(edge, edge, i, j);        /*DSA*/printf("done\n");
	}
}

int main(int argc, char* argv[])
{
	GraphMatrix<char, int> g;
	importGraph(g);

	return 0;

}