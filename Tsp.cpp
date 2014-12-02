#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
using namespace std;

/******************************************************************************************
* 测试、演示用小工具
******************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <typeinfo.h>

#if defined(DSA_DEBUG) //编译开关，控制调试输出
#else
#define DSA(x) //不输出
#endif


/******************************************************************************************
* 列表、向量等结构内的节点中，可以存放基本类型或构造类型
* 按照本书约定，出于效率的考虑，对于后一情况通常只保存对象的指针
* 因此，在主体结构析构之前，需要首先释放这些成员对象所占的空间
* 此处，借助C++中偏特化技术区分上述两种情况，并做对应处理
******************************************************************************************/

template <typename T> struct Cleaner {
	static void clean(T x) { //相当于递归基
#ifdef _DEBUG
		static int n = 0;
		if (7 > strlen(typeid (T).name())) { //复杂类型一概忽略，只输出基本类型
			printf("\t<%s>[%d]=", typeid (T).name(), ++n);
			print(x);
			printf(" purged\n");
		}
#endif
	}
};

template <typename T> struct Cleaner<T*> {
	static void clean(T* x) {
		if (x) { delete x; } //如果其中包含指针，递归释放
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

typedef int Rank; //秩
#define DEFAULT_CAPACITY  3 //默认的初始容量（实际应用中可设置为更大）

template <typename T> class Vector { //向量模板类
protected:
	Rank _size; int _capacity;  T* _elem; //规模、容量、数据区
	void copyFrom(T const* A, Rank lo, Rank hi); //复制数组区间A[lo, hi)
	void expand(); //空间不足时扩容
	void shrink(); //装填因子过小时压缩
	//bool bubble(Rank lo, Rank hi); //扫描交换
	//void bubbleSort(Rank lo, Rank hi); //起泡排序算法
	//Rank max(Rank lo, Rank hi); //选取最大元素
	//void selectionSort(Rank lo, Rank hi); //选择排序算法
	//void merge(Rank lo, Rank mi, Rank hi); //归并算法
	//void mergeSort(Rank lo, Rank hi); //归并排序算法
	//Rank partition(Rank lo, Rank hi); //轴点构造算法
	//void quickSort(Rank lo, Rank hi); //快速排序算法
	//void heapSort(Rank lo, Rank hi); //堆排序（稍后结合完全堆讲解）
public:
	// 构造函数
	Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0) //容量为c、规模为s、所有元素初始为v
	{
		_elem = new T[_capacity = c]; for (_size = 0; _size < s; _elem[_size++] = v);
	} //s<=c
	Vector(T const* A, Rank n) { copyFrom(A, 0, n); } //数组整体复制
	Vector(T const* A, Rank lo, Rank hi) { copyFrom(A, lo, hi); } //区间
	Vector(Vector<T> const& V) { copyFrom(V._elem, 0, V._size); } //向量整体复制
	Vector(Vector<T> const& V, Rank lo, Rank hi) { copyFrom(V._elem, lo, hi); } //区间
	//// 析构函数
	~Vector() { delete[] _elem; } //释放内部空间
	//// 只读访问接口
	Rank size() const { return _size; } //规模
	bool empty() const { return !_size; } //判空
	//int disordered() const; //判断向量是否已排序
	//Rank find(T const& e) const { return find(e, 0, _size); } //无序向量整体查找
	//Rank find(T const& e, Rank lo, Rank hi) const; //无序向量区间查找
	//Rank search(T const& e) const //有序向量整体查找
	//{
	//	return (0 >= _size) ? -1 : search(e, 0, _size);
	//}
	//Rank search(T const& e, Rank lo, Rank hi) const; //有序向量区间查找
	//// 可写访问接口
	T& operator[] (Rank r) const; //重载下标操作符，可以类似于数组形式引用各元素
	Vector<T> & operator= (Vector<T> const&); //重载赋值操作符，以便直接克隆向量
	T remove(Rank r); //删除秩为r的元素
	int remove(Rank lo, Rank hi); //删除秩在区间[lo, hi)之内的元素
	Rank insert(Rank r, T const& e); //插入元素
	Rank insert(T const& e) { return insert(_size, e); } //默认作为末元素插入
	//void sort(Rank lo, Rank hi); //对[lo, hi)排序
	//void sort() { sort(0, _size); } //整体排序
	//void unsort(Rank lo, Rank hi); //对[lo, hi)置乱
	//void unsort() { unsort(0, _size); } //整体置乱
	//int deduplicate(); //无序去重
	//int uniquify(); //有序去重
	//// 遍历
	//void traverse(void(*) (T&)); //遍历（使用函数指针，只读或局部性修改）
	//template <typename VST> void traverse(VST&); //遍历（使用函数对象，可全局性修改）
}; //Vector

template <typename T> T& Vector<T>::operator[] (Rank r) const //重载下标操作符
{
	return _elem[r];
} // assert: 0 <= r < _size

template <typename T> Vector<T>& Vector<T>::operator= (Vector<T> const& V) { //重载
	if (_elem) delete[] _elem; //释放原有内容
	copyFrom(V._elem, 0, V.size()); //整体复制
	return *this; //返回当前对象的引用，以便链式赋值
}

template <typename T> //将e作为秩为r元素插入
Rank Vector<T>::insert(Rank r, T const& e) { //assert: 0 <= r <= size
	expand(); //若有必要，扩容
	for (int i = _size; i > r; i--) _elem[i] = _elem[i - 1]; //自后向前，后继元素顺次后移一个单元
	_elem[r] = e; _size++; //置入新元素并更新容量
	return r; //返回秩
}

template <typename T> void Vector<T>::expand() { //向量空间不足时扩容
	if (_size < _capacity) return; //尚未满员时，不必扩容
	if (_capacity < DEFAULT_CAPACITY) _capacity = DEFAULT_CAPACITY; //不低于最小容量
	T* oldElem = _elem;  _elem = new T[_capacity <<= 1]; //容量加倍
	for (int i = 0; i < _size; i++)
		_elem[i] = oldElem[i]; //复制原向量内容（T为基本类型，或已重载赋值操作符'='）
	/*DSA*/ //printf("\n_ELEM [%x]*%d/%d expanded and shift to [%x]*%d/%d\n", oldElem, _size, _capacity/2, _elem, _size, _capacity);
	delete[] oldElem; //释放原空间
}

template <typename T> //元素类型
void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi) { //以数组区间A[lo, hi)为蓝本复制向量
	_elem = new T[_capacity = 2 * (hi - lo)]; _size = 0; //分配空间，规模清零
	while (lo < hi) //A[lo, hi)内的元素逐一
		_elem[_size++] = A[lo++]; //复制至_elem[0, hi - lo)
}

template <typename T> int Vector<T>::remove(Rank lo, Rank hi) { //删除区间[lo, hi)
	if (lo == hi) return 0; //出于效率考虑，单独处理退化情况，比如remove(0, 0)
	while (hi < _size) _elem[lo++] = _elem[hi++]; //[hi, _size)顺次前移hi - lo个单元
	_size = lo; //更新规模，直接丢弃尾部[lo, _size = hi)区间
	shrink(); //若有必要，则缩容
	return hi - lo; //返回被删除元素的数目
}

template <typename T> T Vector<T>::remove(Rank r) { //删除向量中秩为r的元素，0 <= r < size
	T e = _elem[r]; //备份被删除元素
	remove(r, r + 1); //调用区间删除算法，等效于对区间[r, r + 1)的删除
	return e; //返回被删除元素
}

template <typename T> void Vector<T>::shrink() { //装填因子过小时压缩向量所占空间
	if (_capacity < DEFAULT_CAPACITY << 1) return; //不致收缩到DEFAULT_CAPACITY以下
	if (_size << 2 > _capacity) return; //以25%为界
	T* oldElem = _elem;  _elem = new T[_capacity >>= 1]; //容量减半
	for (int i = 0; i < _size; i++) _elem[i] = oldElem[i]; //复制原向量内容
	delete[] oldElem; //释放原空间
}


/*****************
* Stack @ Vector
******************/
template <typename T> class Stack : public Vector<T> { //将向量的首/末端作为栈底/顶
public: //size()、empty()以及其它开放接口，均可直接沿用
	void push(T const& e) { insert(size(), e); } //入栈：等效于将新元素作为向量的末元素插入
	T pop() { return remove(size() - 1); } //出栈：等效于删除向量的末元素
	T& top() { return (*this)[size() - 1]; } //取顶：直接返回向量的末元素
};

/*****************
* Class Graph
******************/
#pragma once

typedef enum { UNDISCOVERED, DISCOVERED, VISITED } VStatus; //顶点状态
typedef enum { UNDETERMINED, TREE, CROSS, FORWARD, BACKWARD } EType; //边在遍历树中所属的类型

template <typename Tv, typename Te> //顶点类型、边类型
class Graph { //图Graph模板类
private:
	void reset() { //所有顶点、边的辅助信息复位
		for (int i = 0; i < n; i++) { //所有顶点的
			status(i) = UNDISCOVERED; dTime(i) = fTime(i) = -1; //状态，时间标签
			parent(i) = -1; priority(i) = INT_MAX; //（在遍历树中的）父节点，优先级数
			for (int j = 0; j < n; j++) //所有边的
			if (exists(i, j)) type(i, j) = UNDETERMINED; //类型
		}
	}
	//void BFS(int, int&); //（连通域）广度优先搜索算法
	//void DFS(int, int&); //（连通域）深度优先搜索算法
	//void BCC(int, int&, Stack<int>&); //（连通域）基于DFS的双连通分量分解算法
	bool TSort(int, int&, Stack<Tv>*); //（连通域）基于DFS的拓扑排序算法
	//template <typename PU> void PFS(int, PU); //（连通域）优先级搜索框架
public:
	// 顶点
	int n; //顶点总数
	virtual int insert(Tv const&) = 0; //插入顶点，返回编号
	virtual Tv remove(int) = 0; //删除顶点及其关联边，返回该顶点信息
	virtual Tv& vertex(int) = 0; //顶点v的数据（该顶点的确存在）
	virtual int inDegree(int) = 0; //顶点v的入度（该顶点的确存在）
	virtual int outDegree(int) = 0; //顶点v的出度（该顶点的确存在）
	virtual int firstNbr(int) = 0; //顶点v的首个邻接顶点
	virtual int nextNbr(int, int) = 0; //顶点v的（相对于顶点j的）下一邻接顶点
	virtual VStatus& status(int) = 0; //顶点v的状态
	virtual int& dTime(int) = 0; //顶点v的时间标签dTime
	virtual int& fTime(int) = 0; //顶点v的时间标签fTime
	virtual int& parent(int) = 0; //顶点v在遍历树中的父亲
	virtual int& priority(int) = 0; //顶点v在遍历树中的优先级数
	// 边：这里约定，无向边均统一转化为方向互逆的一对有向边，从而将无向图视作有向图的特例
	int e; //边总数
	virtual bool exists(int, int) = 0; //边(v, u)是否存在
	virtual void insert(Te const&, int, int, int) = 0; //在顶点v和u之间插入权重为w的边e
	virtual Te remove(int, int) = 0; //删除顶点v和u之间的边e，返回该边信息
	virtual EType & type(int, int) = 0; //边(v, u)的类型
	virtual Te& edge(int, int) = 0; //边(v, u)的数据（该边的确存在）
	virtual int& weight(int, int) = 0; //边(v, u)的权重
	// 算法
	void bfs(int); //广度优先搜索算法
	void dfs(int); //深度优先搜索算法
	void bcc(int); //基于DFS的双连通分量分解算法
	Stack<Tv>* tSort(int); //基于DFS的拓扑排序算法
	void prim(int); //最小支撑树Prim算法
	void dijkstra(int); //最短路径Dijkstra算法
	template <typename PU> void pfs(int, PU); //优先级搜索框架
};

/*****************
* grap tsort
******************/
#pragma once

template <typename Tv, typename Te> //基于DFS的拓扑排序算法
Stack<Tv>* Graph<Tv, Te>::tSort(int s) { //assert: 0 <= s < n
	reset(); int clock = 0; int v = s;
	Stack<Tv>* S = new Stack<Tv>; //用栈记录排序顶点
	do {
		if (UNDISCOVERED == status(v))
		if (!TSort(v, clock, S)) { //clock并非必需
			/*DSA*/print(S);
			while (!S->empty()) //任一连通域（亦即整图）非DAG
				S->pop(); break; //则不必继续计算，故直接返回
		}
	} while (s != (v = (++v % n)));
	return S; //若输入为DAG，则S内各顶点自顶向底排序；否则（不存在拓扑排序），S空
}

template <typename Tv, typename Te> //基于DFS的拓扑排序算法（单趟）
bool Graph<Tv, Te>::TSort(int v, int& clock, Stack<Tv>* S) { //assert: 0 <= v < n
	dTime(v) = ++clock; status(v) = DISCOVERED; //发现顶点v
	for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u)) //枚举v的所有邻居u
		switch (status(u)) { //并视u的状态分别处理
		case UNDISCOVERED:
			parent(u) = v; type(v, u) = TREE;
			if (!TSort(u, clock, S)) //从顶点u处出发深入搜索
				return false; //若u及其后代不能拓扑排序（则全图亦必如此），故返回并报告
			break;
		case DISCOVERED:
			type(v, u) = BACKWARD; //一旦发现后向边（非DAG），则
			return false; //不必深入，故返回并报告
		default: //VISITED (digraphs only)
			type(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS;
			break;
	}
	status(v) = VISITED; S->push(vertex(v)); //顶点被标记为VISITED时，随即入栈
	return true; //v及其后代可以拓扑排序
}


/*****************
* Class GraphMatrix
******************/

template <typename Tv> struct Vertex { //顶点对象（为简化起见，并未严格封装）
	Tv data; int inDegree, outDegree; VStatus status; //数据、出入度数、状态
	int dTime, fTime; //时间标签
	int parent; int priority; //在遍历树中的父节点、优先级数
	Vertex(Tv const& d = (Tv)0) : //构造新顶点
		data(d), inDegree(0), outDegree(0), status(UNDISCOVERED),
		dTime(-1), fTime(-1), parent(-1), priority(INT_MAX) {} //暂不考虑权重溢出
};

template <typename Te> struct Edge { //边对象（为简化起见，并未严格封装）
	Te data; int weight; EType type; //数据、权重、类型
	Edge(Te const& d, int w) : data(d), weight(w), type(UNDETERMINED) {} //构造
};


template <typename Tv, typename Te> //顶点类型、边类型
class GraphMatrix : public Graph<Tv, Te> { //基于向量，以邻接矩阵形式实现的图
private:
	Vector< Vertex< Tv > > V; //顶点集（向量）
	Vector< Vector< Edge< Te > * > > E; //边集（邻接矩阵）
public:
	GraphMatrix() { n = e = 0; } //构造
	~GraphMatrix() { //析构
		for (int j = 0; j < n; j++) //所有动态创建的
		for (int k = 0; k < n; k++) //边记录
			delete E[j][k]; //逐条清除
	}
	// 顶点的基本操作：查询第i个顶点（0 <= i < n）
	virtual Tv& vertex(int i) { return V[i].data; } //数据
	virtual int inDegree(int i) { return V[i].inDegree; } //入度
	virtual int outDegree(int i) { return V[i].outDegree; } //出度
	virtual int firstNbr(int i) { return nextNbr(i, n); } //首个邻接顶点
	virtual int nextNbr(int i, int j) //相对于顶点j的下一邻接顶点（改用邻接表可提高效率）
	{
		while ((-1 < j) && (!exists(i, --j))); return j;
	} //逆向线性试探
	virtual VStatus& status(int i) { return V[i].status; } //状态
	virtual int& dTime(int i) { return V[i].dTime; } //时间标签dTime
	virtual int& fTime(int i) { return V[i].fTime; } //时间标签fTime
	virtual int& parent(int i) { return V[i].parent; } //在遍历树中的父亲
	virtual int& priority(int i) { return V[i].priority; } //在遍历树中的优先级数
	// 顶点的动态操作
	virtual int insert(Tv const& vertex) { //插入顶点，返回编号
		for (int j = 0; j < n; j++) E[j].insert(NULL); n++; //各顶点预留一条潜在的关联边
		E.insert(Vector<Edge<Te>*>(n, n, (Edge<Te>*) NULL)); //创建新顶点对应的边向量
		return V.insert(Vertex<Tv>(vertex)); //顶点向量增加一个顶点
	}
	virtual Tv remove(int i) { //删除第i个顶点及其关联边（0 <= i < n）
		for (int j = 0; j < n; j++) //所有出边
		if (exists(i, j)) { delete E[i][j]; V[j].inDegree--; } //逐条删除
		E.remove(i); n--; //删除第i行
		Tv vBak = vertex(i); V.remove(i); //删除顶点i
		for (int j = 0; j < n; j++) //所有入边
		if (Edge<Te> * e = E[j].remove(i)) { delete e; V[j].outDegree--; } //逐条删除
		return vBak; //返回被删除顶点的信息
	}
	// 边的确认操作
	virtual bool exists(int i, int j) //边(i, j)是否存在
	{
		return (0 <= i) && (i < n) && (0 <= j) && (j < n) && E[i][j] != NULL;
	}
	// 边的基本操作：查询顶点i与j之间的联边（0 <= i, j < n且exists(i, j)）
	virtual EType & type(int i, int j) { return E[i][j]->type; } //边(i, j)的类型
	virtual Te& edge(int i, int j) { return E[i][j]->data; } //边(i, j)的数据
	virtual int& weight(int i, int j) { return E[i][j]->weight; } //边(i, j)的权重
	// 边的动态操作
	virtual void insert(Te const& edge, int w, int i, int j) { //插入权重为w的边e = (i, j)
		if (exists(i, j)) return; //确保该边尚不存在
		E[i][j] = new Edge<Te>(edge, w); //创建新边
		e++; V[i].outDegree++; V[j].inDegree++; //更新边计数与关联顶点的度数
	}
	virtual Te remove(int i, int j) { //删除顶点i和j之间的联边（exists(i, j)）
		Te eBak = edge(i, j); delete E[i][j]; E[i][j] = NULL; //备份后删除边记录
		e--; V[i].outDegree--; V[j].inDegree--; //更新边计数与关联顶点的度数
		return eBak; //返回被删除边的信息
	}
};


/******************************************************************************************
* 数据元素、数据结构通用输出接口
******************************************************************************************/
template <typename T> static void print(T* x) { x ? print(*x) : printf(" <NULL>"); }
template <typename T> static void print(T& x) { UniPrint::p(x); }
template <typename T> static void print(const T& x) { UniPrint::p(x); } //for Stack
static void print(char* x) { printf(" %s", x ? x : "<NULL>"); } //字符串特别处理
static void print(const char* x) { printf(" %s", x ? x : "<NULL>"); } //字符串特别处理

class UniPrint {
public:
	static void p(int);
	static void p(float);
	static void p(double);
	static void p(char);
	//static void p(HuffChar&); //Huffman（超）字符
	static void p(VStatus); //图顶点的状态
	static void p(EType); //图边的类型


	template <typename Tv, typename Te> static void p(GraphMatrix<Tv, Te>&); //Graph
	template <typename T> static void p(T&); //向量、列表等支持traverse()遍历操作的线性结构
	template <typename T> static void p(T* s) //所有指针
	{
		s ? p(*s) : print("<NULL>");
	} //统一转为引用
}; //UniPrint

/******************************************************************************************
* 基本类型
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
* 图Graph
******************************************************************************************/
template <typename Tv, typename Te> //顶点类型、边类型
void UniPrint::p(GraphMatrix<Tv, Te>& s) { //引用
	int inD = 0; for (int i = 0; i < s.n; i++) inD += s.inDegree(i);
	int outD = 0; for (int i = 0; i < s.n; i++) outD += s.outDegree(i);
	printf("%s[%d]*(%d, %d):\n", typeid (s).name(), &s, s.n, s.e); //基本信息
	// 标题行
	print(s.n); printf(" "); print(inD); printf("|");
	for (int i = 0; i < s.n; i++) { print(s.vertex(i)); printf("["); print(s.status(i)); printf("] "); }
	printf("\n");
	// 标题行（续）
	print(outD); printf(" "); print(s.e); printf("|");
	for (int i = 0; i < s.n; i++) { print(s.inDegree(i)); printf(" "); }
	printf("| dTime fTime Parent Weight\n");
	// 水平分隔线
	printf("-----------+"); for (int i = 0; i < s.n; i++) printf("------");
	printf("+----------------------------\n");
	// 逐行输出各顶点
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
* 向量、列表等支持traverse()遍历操作的线性结构
******************************************************************************************/
template <typename T> //元素类型
void UniPrint::p(T& s) { //引用
	printf("%s[%d]*%d:\n", typeid (s).name(), &s, s.size()); //基本信息
	s.traverse(print); //通过print()遍历输出所有元素
	printf("\n");
}



void importGraph(GraphMatrix<char, int> & g) {
	int n; scanf("%d\n", &n); /*DSA*/printf("%d vertices\n", n);
	for (int i = 0; i < n; i++) { //插入v个顶点
		char vertex; scanf("%c", &vertex);  /*DSA*/printf("Inserting vertex"); print(vertex); printf(" ...");
		g.insert(vertex);                  /*DSA*/printf("done\n"); print(g);
	}
	for (int i = 0; i < n; i++)
	for (int j = 0; j < n; j++) { //插入边
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