#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
		if(temp->val == '0' && temp1==0) {
			// 030.3000 처럼 앞에 0이 있는 경우 print 제외
		} else if(temp->val =='.') {
			temp= temp->next;
			k--;
			break;
			// .이 나온 경우 일단 print 중단.
		} else {
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
		if(temp->val == '+' || temp->val == '-'|| temp->val == '('|| temp->val == ')') {
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
		for (ii=0;ii<temp1+1;ii++) {
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
		} else if (val == '+' || val == '-') {
			// (Pop하여 연산기호일 경우 list에 저장, 아닐경우 다시 PUSH) & 현재 연산기호 PUSH
			if (sign->topVal != NULL) {
				OPN *data = pop(sign);
				if (data->Operator->val == '+' || data->Operator->val == '-') {
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
		if(val == '+' || val == '-') {
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
			// 자릿수 맞춰주는 작업
			//8printf("====== 자릿수 맞춰주는 작업 ======\n");
			//printf("opn1 : 정수 %d  소수 %d\n", opn1->intPart, opn1->frcPart);
			//printf("opn1 : 마지막 자리 %c\n", opn1->Operator->val);
			//printf("opn2 : 정수 %d  소수 %d\n", opn2->intPart, opn2->frcPart);
			//printf("opn2 : 마지막 자리 %c\n", opn2->Operator->val);
			//printf("==================================\n");
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
			} else {
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
	// opn2 + opn1 형태로 들어옴. 
	int intP = opn1->intPart;
	int frcP = opn1->frcPart;
	// 자릿수
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
			now2->val = (num3 % 10) + 48;
			if((char)horse->val == '.') {
				upcount = 1;
			} else {
				horse->val += 1;
			}
		} else {
			now2->val = num3 + 48;
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
			horse->val += 1;
			upcount = 0;
			now2 = now2->prev;
			now1 = now1->prev;
		} else {
			now2->val = num3 + 48;
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

void main() {
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
}
