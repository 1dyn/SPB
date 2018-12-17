#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Node {
	char val;
	struct Node *prev;
	struct Node *next;
}
Node;
// 노드
typedef struct DLL {
	Node *head;
	int size;
}
DLL;
// DLL pointer
typedef struct OPNode {
	struct Node *Operator;
	struct OPN *prev;
	struct OPN *next;
	int negative;
	// 음수 양수 체크  
	int intPart;
	// 정수부분 자릿수
	int frcPart;
	// 소수부분 자릿수
}
OPN;
// 연산자와 피연산자 포인팅용 노드
typedef struct OP {
	OPN *head;
	int size;
}
OP;
typedef struct StackNode {
	OPN * val;
	struct StackNode *prev;
}
StackNode;
// 스택 노드
typedef struct Stack {
	StackNode *top;
	OPN* topVal;
}
Stack;
// 스택스택
Node *newnode(char n) {
	Node *temp = (Node *)malloc(sizeof(Node));
	temp->val = n;
	temp->prev = NULL;
	temp->next = NULL;
	return temp;
}
// 인자로 char를 받는 이유
// 포인터 받는 것 보다 나을 것 같음
DLL *newDLL() {
	DLL *temp = (DLL *)malloc(sizeof(DLL));
	temp->head = NULL;
	temp->size = 0;
	return temp;
}
OPN *newOPN(Node *nn, int intP, int frcP, int ngt) {
	OPN *temp = (OPN *)malloc(sizeof(OPN));
	temp->Operator = nn;
	temp->intPart = intP;
	temp->frcPart = frcP;
	temp->negative = ngt;
	temp->next = NULL;
	temp->prev = NULL;
	return temp;
}
// 피연산자 / 연산자 포인터 노드
OP *newOP() {
	OP *temp = (OP *)malloc(sizeof(OP));
	temp->head = NULL;
	temp->size = 0;
	return temp;
}
// 피연산자/ 연산자 포인팅 용도
StackNode *newStackNode(OPN *n) {
	StackNode *temp = (StackNode *)malloc(sizeof(StackNode));
	temp->val = n;
	temp->prev = NULL;
	return temp;
}
Stack *newStack() {
	Stack *temp = (Stack *)malloc(sizeof(Stack));
	temp->top = NULL;
	temp->topVal = NULL;
	return temp;
}
// typedef
// typedef
// typedef
long long getElapsedTime(unsigned int nFlag)
{
	const long long NANOS = 1000000000LL;
	static struct timespec startTS, endTS;
	static long long retDiff = 0;

	if (nFlag == 0) {
		retDiff = 0;
		if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &startTS) == -1) {
			printf("Failed to call clock_gettime\n");
		}
	}
	else {
		if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &endTS) == -1) {
			printf("Failed to call clock_gettime\n");
		}
		retDiff = NANOS * (endTS.tv_sec - startTS.tv_sec) + (endTS.tv_nsec - startTS.tv_nsec);
 	}

	return retDiff;
}


void append(DLL *list, char n) {
	Node* now = list->head;
	Node* nn = newnode(n);
	//printf("%c", nn->val);
	if(list->head == NULL) {
		list->head = nn;
		list->size++;
		return;
	}
	while(now->next != NULL) {
		now = now->next;
	}
	now->next = nn;
	nn->prev = now;
	list->size++;
}
// 값을 바로 넣어줄 수 있는 DLL Append
void OPappend(OP *list, Node *nn, int intP, int frcP, int ngt) {
	// nn이 DLL에 있는 마지막 자릿값 주
	if(frcP == 0 && intP != 0) {
		Node *dot = newnode('.');
		Node *zero = newnode('0');
		dot->next = zero;
		zero->prev = dot;
		if(nn->next != NULL) {
			Node *temp = nn->next;
			nn->next = dot;
			dot->prev = nn;
			zero->next = temp;
			temp->prev = zero;
		} else {
			nn->next = dot;
			dot->prev = nn;
		}
		nn = zero;
		frcP = 1;
	}
	OPN* now = list->head;
	OPN* n = newOPN(nn, intP, frcP, ngt);
	if(list->head == NULL) {
		list->head = n;
		list->size++;
		return;
	}
	while(now->next != NULL) {
		now = now->next;
	}
	now->next = n;
	n->prev = now;
	now = n;
	list->size++;
}
// test
void insert(OPN *node, int index) {
	Node *now = node->Operator;
	if (index <= 0) {
		node->frcPart -= index;
		while (index < 0) {
			Node* a = newnode('0');
			if (now->next == NULL) {
				now->next = a;
				a->prev = now;
			} else {
				Node *temp = now->next;
				now->next = a;
				a->prev = now;
				a->next = temp;
				temp->prev = a;
			}
			now = now->next;
			node->Operator = node->Operator->next;
			index++;
		}
	} else {
		int i = node->intPart + node->frcPart;
		node->intPart += index;
		while (i > 0) {
			now = now->prev;
			i--;
		}
		while (index > 0) {
			Node* a = newnode('0');
			now->prev = a;
			a->next = now;
			now = now->prev;
			index--;
		}
	}
}
// 자릿수 맞추는 함수
void push(Stack *SL, OPN *n) {
	StackNode *nn = newStackNode(n);
	if(SL->top == NULL) {
		SL->top = nn;
		SL->topVal = nn->val;
	} else {
		nn->prev = SL->top;
		SL->top = nn;
		SL->topVal = nn->val;
	}
}
OPN *pop(Stack *SL) {
	if(SL->top == NULL) {
		return NULL;
	}
	OPN *val = SL->topVal;
	if(SL->top->prev == NULL) {
		SL->top = NULL;
		return val;
	} else {
		StackNode *temp = SL->top->prev;
		SL->top = temp;
		SL->topVal = temp->val;
		return val;
	}
}

int MTPnum = 0;

void OPNodePrint(OPN *opn) {
	int intP = opn->intPart;
	int frcP = opn->frcPart;
	int k = intP + frcP;
	Node *temp = opn->Operator;
	if(intP == 1 && frcP == 1 && opn->Operator->val == '0') {
		Node *now = opn->Operator;
		now = now->prev;
		printf("%c", now->prev->val);
		return;
	}
	//단일 정수인 경우 ex) 1.0 
	while(k > 0) {
		temp = temp->prev;
		k--;
	}
	k = intP + frcP;
	int temp1 = 0;
	while(k > 0) {
		if (MTPnum > 1) {//2번 이상 연산
			Node *curNode = temp;
			for(int n=0; n < MTPnum-1; n++) {
				curNode = curNode->next;
			}
			if (curNode->val == '.') {
				break;
			}
		}
		if(temp->val == '0' && temp1==0) {
			// 030.3000 처럼 앞에 0이 있는 경우 print 제외
		}
		else if(temp->val =='.') {
			temp= temp->next;
			k--;
			break;
			// .이 나온 경우 일단 print 중단.
		}
		else {
			temp1 = 1;
			printf("%c", temp->val);
			// 정수를 출력합니다.
		}
		temp= temp->next;
		k--;
		// 이동
	}
	int jj=0;
	// 사실 .은 무조건 있기 때문에...  
	while(temp->next != NULL) {
		if(temp->val == '+' || temp->val == '-' || temp->val == '*'|| temp->val == '('|| temp->val == ')') {
			// 피연산자의  끝인 경우 중단. 
			break;
		}
		if(temp->val != '0') {
			temp1 = jj;
			// 뒤에 0이 아닌 값이 있다면 temp에 기록
		}
		temp = temp->next;
		jj++;
		// 검사
	}
	// . 뒤로 0이 아닌 값이 있는지 없는지 검사.
	if(temp1!=1 || temp1!=2) {
		int ii=0;
		printf(".");
		for (ii=0;ii<jj;ii++) {
			temp = temp->prev;
		}
		for (ii=0;ii<temp1;ii++) {
			printf("%c", temp->val);
			temp = temp->next;
		}
	}
	// .뒤로 0이 아닌 값이 있는 경우 출력.
}
OP *in_to_postfix(OP *inputlist) {
	Stack *sign = newStack();
	// 기호를 저장할 스택인 sign 선언
	OP *list = newOP();
	// 후위표기법으로 바꾼 값을 새롭게 저장하는 OP선언 
	char val;
	// 파일에서 읽어온 한글자를 저장하는 변수 선언
	OPN *now = inputlist->head;
	// 리스트 받아와서 값 넘겨줘야 함
	while (now != NULL) {
		char val = now->Operator->val;
		if (val == '(') {
			push(sign, now);
		} else if (val == ')') {
			if(now->Operator->next != NULL) {
				Node *prev = now->Operator->prev;
				Node *next = now->Operator->next;
				prev->next = next;
				next->prev = prev;
				// DLL 에서 ')'를 제거함
			}
			while (1) {
				OPN *data = pop(sign);
				if (data->Operator->val != '(') {
					OPappend(list, data->Operator, data->intPart, data->frcPart, 0);
				} else {
					if (data->Operator->prev != NULL) {
						Node *prev = data->Operator->prev;
						Node *next = data->Operator->next;
						prev->next = next;
						next->prev = prev;
						OPN *OPNprev = data->prev;
						OPN *OPNnext = data->next;
						OPNprev->next = OPNnext;
						OPNnext->prev = OPNprev;
					} else {
						Node *next = data->Operator->next;
						next->prev = NULL;
					}
					break;
				}
			}
		} else if (val == '+' || val == '-'|| val == '*') {
			// (Pop하여 연산기호일 경우 list에 저장, 아닐경우 다시 PUSH) & 현재 연산기호 PUSH
			if (sign->topVal != NULL) {
				OPN *data = pop(sign);
				if (data->Operator->val == '+' || data->Operator->val == '-' | data->Operator->val == '*') {
					OPappend(list, data->Operator, data->intPart, data->frcPart, data->negative);
					push(sign, now);
				} else {
					push(sign, data);
					push(sign, now);
				}
			} else {
				push(sign, now);
			}
		} else {
			OPappend(list, now->Operator, now->intPart, now->frcPart, now->negative);
		}
		now = now->next;
	}
	while (sign->top != NULL) {
		OPN *data = pop(sign);
		OPappend(list, data->Operator, data->intPart, data->frcPart, data->negative);
	}
	return list;
	// list(후위표기법으로 바꾼 DLL)를 리턴해줌
}
// return = list
void calculate(OP* OP) {
	Stack *stack = newStack();
	// 피 연산자 저장용 스택
	OPN *now = OP->head;
	OPN *ans;
	// OPN 불러옵니다...
	while(now != NULL) {
		char val = now->Operator->val;
		if(val == '+' || val == '-' || val =='*') {
			OPN *opn1 = pop(stack);
			OPN *opn2 = pop(stack);
			int opn1_intP = opn1->intPart;
			int opn1_frcP = opn1->frcPart;
			int opn2_intP = opn2->intPart;
			int opn2_frcP = opn2->frcPart;
			if(opn1_intP > opn2_intP) {
				insert(opn2, opn1_intP - opn2_intP + 1);
				insert(opn1, 1);
				//opn2에 opn1_intP-opn2_intP insert;
			} else {
				//opn1 < opn2;
				insert(opn1, opn2_intP - opn1_intP + 1);
				insert(opn2, 1);
				//opn1에 opn1_intP-opn2_intP insert;
			}
			if(opn1_frcP > opn2_frcP) {
				insert(opn2, opn2_frcP - opn1_frcP - 1);
				insert(opn1, -1);
				//opn2에 opn1_frcP-opn2_frcP insert;
			} else {
				insert(opn1, opn1_frcP - opn2_frcP - 1);
				insert(opn2, -1);
				//opn1에 opn1_frcP-opn2_frcP insert;
			}

			if(val == '+') {
				if (opn2->negative == 0 && opn1->negative == 0) {
					//양수 + 양수
					calADD(opn2,opn1);
					ans = opn1;
				} else if (opn2->negative == 0 && opn1->negative == 1) {
					//양수 + 음수
					opn1->negative = 0;
					calSUB(opn2,opn1);
					ans = opn2;
				} else if (opn2->negative == 1 && opn1->negative == 0) {
					//음수 + 양수
					opn2->negative = 0;
					calSUB(opn1,opn2);
					ans = opn1;
				} else {
					//음수 + 음수
					calADD(opn2,opn1);
					opn2->negative = 1;
					ans = opn2;
				}
				push(stack, ans);
				now = now->next;
			} else if (val == '-'){
				if (opn2->negative == 0 && opn1->negative == 0) {
					//양수 - 양수
					calSUB(opn2,opn1);
					ans = opn2;
				} else if (opn2->negative == 0 && opn1->negative == 1) {
					//양수 - 음수
					opn1->negative = 0;
					calADD(opn2,opn1);
					ans = opn2;
				} else if (opn2->negative == 1 && opn1->negative == 0) {
					//음수 - 양수
					calADD(opn2,opn1);
					opn2->negative = 1;
					ans = opn2;
				} else {
					//음수 - 음수
					calSUB(opn1,opn2);
					opn1->negative = 1;
					ans = opn1;
				}
				push(stack, ans);
				now = now->next;
			} else {
				// 곱하기인 경우...
				calMTP(opn1, opn2);
				ans = opn2;
				push(stack, ans);
				now = now->next;
			}
			// 스택에서 2개의 피연산자를 꺼내 계산해야 한다.
		} else {
			push(stack, now);
			now = now->next;
			// 피 연산자는 스택에 넣어둔다.
		}
	}
	printf("Final Answer is ");
	if (ans->negative == 1) {
		printf("-");
	}
	OPNodePrint(ans);
}

void calADD(OPN* opn1, OPN* opn2) {
	// opn2 + opn1 ?濡??ㅼ?? 
	int intP = opn1->intPart;
	int frcP = opn1->frcPart;
	// ?由??
	if(opn1->negative){
		opn1->negative = 0;
		calSUB(opn2, opn1);
		return;
	} else if (opn2->negative){
		opn2->negative =0;
		calSUB(opn1, opn2);
		return;
	}
	int i, num1, num2, num3;
	Node *now1 = opn1->Operator;
	Node *now2 = opn2->Operator;
	int upcount = 0;
	for (i=0; i<frcP; i++) {
		num2 = now2->val - 48;
		num1 = now1->val - 48;
		num3 = num1 + num2;
		if(num3 > 9) {
			Node *horse = now2->prev;
			Node *horse2 = now1->prev;
			 
			now2->val = (num3 % 10) + 48;
			now1->val = now2->val;
			
			if((char)horse->val == '.') {
				upcount = 1;
			} else {
				horse->val += 1;
				horse2->val += 1;
			}
		} else {
			now2->val = num3 + 48;
			now1->val = now2;
		}
		now2 = now2->prev;
		now1 = now1->prev;
	}
	if(now2->val == '.') {
		now2 = now2->prev;
	}
	if(now1->val == '.') {
		now1 = now1->prev;
	}
	//printf("%c %c", now1->val, now2->val);
	for (i=0; i<intP; i++) {
		num1 = now1->val;
		num2 = now2->val;
		num3 = num1 + num2 - 96;
		if(i==0) {
			num3 += upcount;
		}
		if(num3 > 9) {
			//printf("ERROR? : %c", now2->val);
			Node *horse = now2->prev;
			now2->val = (num3 % 10) + 48;
			now1->val = now2->val;
			horse->val += 1;
			
			upcount = 0;
			now2 = now2->prev;
			now1 = now1->prev;
		} else {
			now2->val = num3 + 48;
			now1->val = now2->val;
			if(now2->prev != NULL) {
				now2 = now2->prev;
				now1 = now1->prev;
			}
		}
	}
}

void calSUB(OPN* opn2, OPN* opn1) {
	//opn2 - opn1
	int o1 = opn2->intPart + opn2->frcPart;
	int o2 = o1;
	int i, num1, num2, num3;
	Node *now1;
	Node *now2;

	now1 = opn1->Operator;
	now2 = opn2->Operator;
	
	for(i=0; i<o1; i++){
		now1 = now1->prev;
		now2 = now2->prev;
	}
	
	while(o2 > 0){
		num1 = now1->val;
		num2 = now2->val;
		num3 = num2 - num1;
		if(num3 < 0){
			if(o2 == o1 -1){
				// 맨 첫자리 수 부터 문제가 생긴 경우
				opn2->negative = 1;
				calSUB(opn1, opn2);
				// 일단 바꿔버리고 계산. 
				return;
			} else if (now2->prev->val == '0'){
				// 문제 잇는 경우 
				int ttt = 1;
				now2 = now2->prev;
				now1 = now1->prev;
				while(now2->val == '0' || now2->val == '.'){
					ttt+=1;
					now2 = now2->prev;
					now1 = now1->prev;
				}
				now2->val = now2->val - 1;
				now1->val = now2->val;
				for(i=0; i<ttt; i++){
					now2 = now2->next;
					now1 = now1->next;
					if(now2->val != '.'){
						now2->val = '9';
						now1->val = now2->val;
					}
				}
				now2->val = num3 + 48 + 10;
				now1->val = now2->val;
				
			} else {
				now2->prev->val -= 1;
				now1->prev->val = now2->prev->val;
				
				now2->val += 10 + num3;
				now1->val = now2->val;
			}
		} else {
			now2->val = num3 + 48;
			now1->val = now2->val;
		}
		
		now1 = now1->next;
		now2 = now2->next;
		
		if(now2->val == '.'){
			now1 = now1->next;
			now2 = now2->next;
			o2--;
		}
		o2--;
	}
}



void calMTP(OPN* opn1, OPN* opn2){
	// 음수 처리는 연산이 끝나고 할 예정.
	/* 계산 순서
	  1. 피 연산자 양 끝의 0과 .을 제거한 새로운 노드를 만들어 둔다.
	  2. 끝 부분 부터 연산을 시작하며, 각 자릿수에 곱해주는 형태로 진행한다. 
	  3. 또한 연산을 진행하면서 값이 저장될 새로운 Node도 필요하다.
	  4. 연산이 끝나면 소수 자릿수에 맞추어 .을 넣어주는 방식으로 진행한다. 
	*/
	
	DLL *trimOPN1 = newDLL();
	DLL *trimOPN2 = newDLL();
	DLL *trimTEMP = newDLL();
	DLL *trimResult = newDLL(); 
	int i, j = opn1->frcPart + opn1->intPart, cnt=0, check=0;
	
	// TRIM 
	Node *trimNode1 = opn1->Operator;
	Node *trimNode2 = opn2->Operator;
	
	for(i=0; i<j-1; i++){
		trimNode1 = trimNode1->prev;
		trimNode2 = trimNode2->prev;
	}
	for(i=0;i<j;i++){
		//trimOPN(DLL)에 추가해준다.
		//새 TRIM도 마찬가지. 여기서 소숫점은 신경쓰지 말자. 어차피 알아서 된다. 
		if(check==0){
			cnt+=1;
			if(trimNode1->val != '0' && trimNode2->val != '0'){
				if(trimNode1->val != '.' ){
					append(trimOPN1, trimNode1->val);
				}
				if(trimNode2->val != '.' ){
					append(trimOPN2, trimNode2->val);
				}
			} else if(trimNode1->val == '.'){
				check = 1;
			} else if(trimNode2->val == '.'){
				check = 1;
			} else {
				check = 1;
				append(trimOPN1, trimNode1->val);
				append(trimOPN2, trimNode2->val);
			}
		} else {
			if(trimNode1->val != '.' ){
				append(trimOPN1, trimNode1->val);
			}
			if(trimNode2->val != '.' ){
				append(trimOPN2, trimNode2->val);
			}
		} 
		trimNode1 = trimNode1->next;
		trimNode2 = trimNode2->next;
	}
	// trim 작업을 통해 소수 자릿수와 새DLL들을 만들었음.
	// 현재 trimNode들은 각 자릿수의 하단 부에 위치함.
	// opn1 * opn2 이므로, opn2의 자리를 이동시켜 곱셈을 진행하도록 함.
	// 그리고 귀찮으니까 trimNode1, trimNode2는 재사용하려 했으나 일단 새로 만듬. 
	
	Node *TPN1 = trimOPN1->head;
	Node *TPN2 = trimOPN2->head;
	
	/*
	
	while(1){
		if(TPN2->prev == NULL){
			break;
		}
		TPN2 = TPN2->prev;
	}
	printf("TPN2 : ");
	while(1){
		printf("%c", TPN2->val);
		if(TPN2->next == NULL){
			break;
		}
		TPN2 = TPN2->next;
	}
	printf("\n");
	
	*/
	
	
	while(TPN1->next != NULL){
		TPN1 = TPN1->next;
	}
	while(TPN2->next != NULL){
		TPN2 = TPN2->next;
	}
	
	int dec = 0, num1=0, num2=0, num3=0, upcount=0, tt=0;
	append(trimTEMP, '0');
	append(trimResult, '0');
	
	Node *TTN = trimTEMP->head;
	Node *TRN = trimResult->head;
	// 계산 결과값이 들어갈 이 친구도...

	while(1){
		num2 = TPN2->val - 48;
		while(1){
			num1 = TPN1->val - 48;
			num3 = num1 * num2;
			upcount = num3/10;

			//printf("%d * %d = %d\n", num1, num2, num3);
			
			TTN->val += (num3 % 10);
			
			if(upcount > 0){
				if(TTN->prev == NULL){
					Node *newn = newnode('0');
					TTN->prev = newn;
					newn->next = TTN;
					//없으면 앞에 0을 채워 줍니다. 
				}
				TTN->prev->val += upcount;
			}
			if(TPN1->prev == NULL){
				break;
			}else {
				TPN1 = TPN1->prev;
			}
			if(TTN->prev == NULL){
				Node *newn = newnode('0');
				TTN->prev = newn;
				newn->next = TTN;
			}
			upcount = 0;
			TTN = TTN->prev;
		}
		//trimtemp에 값을 저장하는 과정 +  
		// 하나의 자릿수 연산이 모두 종료된 상황 
		
		/*
		while(1){
			if(TTN->prev == NULL){
				break;
			}
			TTN = TTN->prev;
		}
		*/
		/*
		printf("TTN : ");
		while(1){
			printf("%c", TTN->val);
			if(TTN->next == NULL){
				break;
			}
			TTN = TTN->next;
		}
		if(TTN->val > 57){
			Node *newn = newnode('1'); 
			TTN->prev = newn;
			newn->next = TTN;
			TTN->val -= 10;
		}
		printf("\n");
		// 혹시 초과했다면.. 늘려주자. 
		*/
		
		while(TTN->next != NULL){
			TTN = TTN->next;
		}
		while(TRN->next != NULL){
			TRN = TRN->next;
		}
		
		/*
		
		printf("TRN : ");
		while(1){
			if(TRN->prev == NULL){
				break;
			}else{
				TRN = TRN->prev;
			}
		}
		while(1){
			printf("%c", TRN->val);
			if(TRN->next == NULL){
				break;
			} else {
				TRN = TRN->next;
			}
		}
		printf("\n");
		*/
		
		for(i=0; i<dec; i++){
			if(TRN->prev == NULL){
				Node *newn = newnode('0'); 
				TRN->prev = newn;
				newn->next = TRN;
			}
			TRN = TRN->prev;
		}
		// 자릿수를 맞춰주기 위해 위치를 조정하는 작업.
		// trimtemp DLL에는 한 자릿수 연산된 값이 저장되어 있음
		// 이제 자릿수를 반영해 trimtemp의 값과 trimResultNode를 더하는 과정만 남음
		
				
		int tr1=0, tr2=0, tr3=0;
		while(1){
			tr1 = TTN->val - 48;
			tr2 = TRN->val - 48;
			tr3 = tr1 + tr2;
			//printf("TR1 : %d\nTR2: %d\n", tr1, tr2);
			if(tr3 > 9){
				TRN->val = tr3 + 38;
				if(TRN->prev == NULL){
					Node *newn = newnode('0'); 
					TRN->prev = newn;
					newn->next = TRN;
				}
				TRN->prev->val = 48 + tr3/10;
			}else{
				TRN->val = tr3 + 48;
			}
			if(TTN->prev != NULL && TRN->prev == NULL){
				Node *newn = newnode('0'); 
				TRN->prev = newn;
				newn->next = TRN;
			}
			if(TTN->prev == NULL){
				break;
			}
			TTN = TTN->prev;
			TRN = TRN->prev;
		}
		// temp + result 덧셈을 진행합니다.
		
		while(TRN->next != NULL){
			TRN = TRN->next;
		}
		while(TTN->next != NULL){
			TTN->val = '0';
			TTN = TTN->next;
		}
		// 연산 종료후 사용했던 애들은 되돌려 놓음. 
		
		dec += 1;
		
		while(1){
			if(TPN1->next == NULL){
				break;
			} else {
				TPN1 = TPN1->next;
			}
		}
		if(TPN2->prev == NULL){
			break;
		}else{
			TPN2 = TPN2->prev;
		}
		
	}
	// 현재 TRN은 맨 오른쪽을 포인팅 하고 있음. 00060 이런느낌.
	// 이제 소수점 계산해봅시다...
	// 소수점 계산도 좀 이상한거 같음... ㅜㅜ  
	int sosu = opn1->frcPart * 2;
	
	for(i=0; i<sosu; i++){
		TRN = TRN->prev;
	}
	Node *newdot = newnode('.');
	newdot->next = TRN->next;
	newdot->prev = TRN;
	TRN->next->prev = newdot;
	TRN->next = newdot;
	
	int IP=0, FP=0, doted=0;
	while(1){
		if(TRN->prev == NULL){
			break;
		}
		TRN = TRN->prev;
	}
	while(1){
		if(TRN->next == NULL){
			break;
		}
		if(TRN->val == '.'){
			doted=1;
		}else{
			if(doted){
				FP+=1;
			}else{
				IP+=1;
			}
		}
		TRN = TRN->next;
	}
	
	trimNode1 = opn2->Operator;
	trimNode2 = opn2->Operator;

	for(i=0; i<j; i++){
		trimNode1 = trimNode1->prev;
	}
	// 이 부분도 문제가 있습니다. 
	if(trimNode1->prev == NULL){
		while(1){
			if(TRN->next == NULL){
				break;
			}
			TRN = TRN->next;
		}
		trimNode2->next->prev = TRN;
		TRN->next = trimNode2->next;
		opn2->Operator = TRN;
		
	} else {
		trimNode1->next->prev = TRN;
		TRN->next = trimNode1->next;
		opn2->Operator = TRN;
	}
	// 재활용 맞음 ㅎㅎ;; 
	
	while(1){
		if(TRN->prev == NULL){
			break;
		}
		TRN = TRN->prev;
	}
	
	opn2->intPart = IP+1;
	opn2->frcPart = FP;
	
	MTPnum += 1;
	
	free(trimOPN1);
	free(trimOPN2);
	free(trimTEMP);
	free(trimResult);
}

void main(const int argc, char **argv)
{
	getElapsedTime(0);
	DLL *list = newDLL();
	// 문자열 (전체 문장) 입력을 위한 리스트
	FILE *input = fopen("input.txt", "r");
	char *str;
	int cnt[2] = {
		0, 0
	}
	;
	int t = 1;
	//정수 자릿수, 소수 자릿수 체크용도
	int ngtCheck = 0;
	// 음수 처리 용도
	int firstngt = 0;
	// 맨 처음에 음수 연산자가 오는경우 처리 
	OP *operand = newOP();
	// 끝자리수 저장하기위한 OP
	while (fscanf(input, "%c", &str) != EOF) {
		if ((char)str != '=') {
			if ((char)str == '.') {
				// str이 '.'일 때
				if(t!=1) {
					printf("소수 입력 오류. \n잘못 입력된 이후의 값은 무시됩니다.\n");
					break;
				}
				t = 0;
				append(list, (char)str);
			} else if ((char)str == '(') {
				append(list, (char)str);
				Node *now = list->head;
				while(now->next != NULL) {
					now = now->next;
				}
				t = 1;
				cnt[0]=0;
				cnt[1]=0;
				if(ngtCheck) {
					OPappend(operand, now, cnt[0], cnt[1], 1);
					ngtCheck = 0;
				} else {
					OPappend(operand, now, cnt[0], cnt[1], 0);
				}
			} else if((char)str == ')') {
				Node *now = list->head;
				while(now->next != NULL) {
					now = now->next;
				}
				if(ngtCheck) {
					OPappend(operand, now, cnt[0], cnt[1], 1);
					ngtCheck = 0;
				} else {
					OPappend(operand, now, cnt[0], cnt[1], 0);
				}
				t = 1;
				cnt[0] = 0;
				cnt[1] = 0;
				while(now->next != NULL) {
					now = now->next;
				}
				append(list, (char)str);
				now = now->next;
			} else if ((char)str == '+') {
				if(list->head != NULL) {
					Node *opTest = list->head;
					while(opTest->next != NULL) {
						opTest = opTest->next;
					}
					if(opTest->val == '+' || opTest->val == '-') {
						printf("연산자 입력 오류\n");
						break;
					}
				}
				t = 1;
				Node *now = list->head;
				while(now->next != NULL) {
					// now포인터를 list의 맨끝으로 이동
					now = now->next;
				}
				if(ngtCheck) {
					OPappend(operand, now, cnt[0], cnt[1], 1);
					ngtCheck = 0;
				} else {
					OPappend(operand, now, cnt[0], cnt[1], 0);
				}
				append(list, (char)str);
				cnt[0] = 0;
				cnt[1] = 0;
				// OP에 숫자 추가 후 cnt 초기화
				while(now->next != NULL) {
					now = now->next;
				}
				if(ngtCheck) {
					OPappend(operand, now, cnt[0], cnt[1], 1);
				} else {
					OPappend(operand, now, cnt[0], cnt[1], 0);
				}
				ngtCheck = 0;
				//free(now);
			} else if ((char)str == '-') {
				if(list->head == NULL) {
					ngtCheck = 1;
				} else {
					Node *opTest = list->head;
					while(opTest->next != NULL) {
						opTest = opTest->next;
					}
					if(opTest->val == '+' || opTest->val == '-') {
						printf("연산자 입력 오류\n");
						break;
					}
					t = 1;
					Node *now = list->head;
					while(now->next != NULL) {
						// now포인터를 list의 맨끝으로 이동
						now = now->next;
					}
					if(now->val == '(') {
						ngtCheck = 1;
					} else {
						if(ngtCheck) {
							OPappend(operand, now, cnt[0], cnt[1], 1);
						} else {
							OPappend(operand, now, cnt[0], cnt[1], 0);
						}
						ngtCheck = 0;
						append(list, (char)str);
						cnt[0] = 0;
						cnt[1] = 0;
						// OP에 숫자 추가 후 cnt 초기화
						while(now->next != NULL) {
							now = now->next;
						}
						if(ngtCheck) {
							OPappend(operand, now, cnt[0], cnt[1], 1);
						} else {
							OPappend(operand, now, cnt[0], cnt[1], 0);
						}
						ngtCheck = 0;
						//free(now);
					}
				}
			} else if ((char)str == '*') {
				if(list->head != NULL) {
					Node *opTest = list->head;
					while(opTest->next != NULL) {
						opTest = opTest->next;
					}
					if(opTest->val == '+' || opTest->val == '-') {
						printf("연산자 입력 오류\n");
						break;
					}
				}
				t = 1;
				Node *now = list->head;
				while(now->next != NULL) {
					// now포인터를 list의 맨끝으로 이동
					now = now->next;
				}
				if(ngtCheck) {
					OPappend(operand, now, cnt[0], cnt[1], 1);
					ngtCheck = 0;
				} else {
					OPappend(operand, now, cnt[0], cnt[1], 0);
				}
				append(list, (char)str);
				cnt[0] = 0;
				cnt[1] = 0;
				// OP에 숫자 추가 후 cnt 초기화
				while(now->next != NULL) {
					now = now->next;
				}
				if(ngtCheck) {
					OPappend(operand, now, cnt[0], cnt[1], 1);
				} else {
					OPappend(operand, now, cnt[0], cnt[1], 0);
				}
				ngtCheck = 0;
				//free(now);
			} else {
				if (t) {
					cnt[0] += 1;
				} else {
					cnt[1] += 1;
				}
				append(list, (char)str);
			}
		}
	}
	Node *now = list->head;
	while (now->next != NULL) {
		now = now->next;
	}
	if(ngtCheck) {
		OPappend(operand, now, cnt[0], cnt[1], 1);
	} else {
		OPappend(operand, now, cnt[0], cnt[1], 0);
	}
	//free(now);
	fclose(input);
	//printf("======[1차가공]중위 표기 수식 ======\n");
	//Oprint(operand);
	//printf("입력 완료 \n");
	operand = in_to_postfix(operand);
	//printf("후위 연산 완료 \n");
	//printf("\n\n======[2차가공]후위 표기 변환======\n");
	//Oprint(operand);
	calculate(operand);
	//printf("계산 완료 \n");
	//print(list);
	printf("\nElapsed Time: %lld\n", getElapsedTime(1));
}
