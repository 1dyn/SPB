#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct Node {
	char val;
	struct Node *prev;
	struct Node *next;
}
Node;
// ���
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
	// ���� ��� üũ  
	int intPart;
	// �����κ� �ڸ���
	int frcPart;
	// �Ҽ��κ� �ڸ���
}
OPN;
// �����ڿ� �ǿ����� �����ÿ� ���
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
// ���� ���
typedef struct Stack {
	StackNode *top;
	OPN* topVal;
}
Stack;
// ���ý���
Node *newnode(char n) {
	Node *temp = (Node *)malloc(sizeof(Node));
	temp->val = n;
	temp->prev = NULL;
	temp->next = NULL;
	return temp;
}
// ���ڷ� char�� �޴� ����
// ������ �޴� �� ���� ���� �� ����
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
// �ǿ����� / ������ ������ ���
OP *newOP() {
	OP *temp = (OP *)malloc(sizeof(OP));
	temp->head = NULL;
	temp->size = 0;
	return temp;
}
// �ǿ�����/ ������ ������ �뵵
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
void append(DLL *list, char n);
void OPappend(OP *list, Node *nn, int intP, int frcP, int ngt);
void insert(OPN *node, int index);
void push(Stack *SL, OPN *n);
void OPNodePrint(OPN *opn);
void calADD(OPN* opn1, OPN* opn2);
void calSUB(OPN* opn2, OPN* opn1);
void calculate(OP* OP);
OPN *pop(Stack *SL);
OP *in_to_postfix(OP *inputlist);
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
// ���� �ٷ� �־��� �� �ִ� DLL Append
void OPappend(OP *list, Node *nn, int intP, int frcP, int ngt) {
	// nn�� DLL�� �ִ� ������ �ڸ��� ��
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
// �ڸ��� ���ߴ� �Լ�
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
	if(opn->negative){
		printf("-");
	}
	Node *temp = opn->Operator;
	if(intP == 1 && frcP == 1 && opn->Operator->val == '0') {
		Node *now = opn->Operator;
		now = now->prev;
		printf("%c", now->prev->val);
		return;
	}
	//���� ������ ��� ex) 1.0 
	while(k > 0) {
		temp = temp->prev;
		k--;
	}
	k = intP + frcP;
	int temp1 = 0;
	while(k > 0) {
		if(temp->val == '0' && temp1==0) {
			// 030.3000 ó�� �տ� 0�� �ִ� ��� print ����
		} else if(temp->val =='.') {
			temp= temp->next;
			k--;
			break;
			// .�� ���� ��� �ϴ� print �ߴ�.
		} else {
			temp1 = 1;
			printf("%c", temp->val);
			// ������ ����մϴ�.
		}
		temp= temp->next;
		k--;
		// �̵�
	}
	int jj=0;
	// ��� .�� ������ �ֱ� ������...  
	while(temp->next != NULL) {
		if(temp->val == '+' || temp->val == '-'|| temp->val == '('|| temp->val == ')') {
			// �ǿ�������  ���� ��� �ߴ�. 
			break;
		}
		if(temp->val != '0') {
			temp1 = jj;
			// �ڿ� 0�� �ƴ� ���� �ִٸ� temp�� ���
		}
		temp = temp->next;
		jj++;
		// �˻�
	}
	// . �ڷ� 0�� �ƴ� ���� �ִ��� ������ �˻�.
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
	// .�ڷ� 0�� �ƴ� ���� �ִ� ��� ���.
}
OP *in_to_postfix(OP *inputlist) {
	Stack *sign = newStack();
	// ��ȣ�� ������ ������ sign ����
	OP *list = newOP();
	// ����ǥ������� �ٲ� ���� ���Ӱ� �����ϴ� OP���� 
	char val;
	// ���Ͽ��� �о�� �ѱ��ڸ� �����ϴ� ���� ����
	OPN *now = inputlist->head;
	// ����Ʈ �޾ƿͼ� �� �Ѱ���� ��
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
				// DLL ���� ')'�� ������
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
			// (Pop�Ͽ� �����ȣ�� ��� list�� ����, �ƴҰ�� �ٽ� PUSH) & ���� �����ȣ PUSH
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
	// list(����ǥ������� �ٲ� DLL)�� ��������
}
// return = list
void calculate(OP* OP) {
	Stack *stack = newStack();
	// �� ������ ����� ����
	OPN *now = OP->head;
	OPN *ans;
	// OPN �ҷ��ɴϴ�...
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
				//opn2�� opn1_intP-opn2_intP insert;
			} else {
				//opn1 < opn2;
				insert(opn1, opn2_intP - opn1_intP + 1);
				insert(opn2, 1);
				//opn1�� opn1_intP-opn2_intP insert;
			}
			if(opn1_frcP > opn2_frcP) {
				insert(opn2, opn2_frcP - opn1_frcP - 1);
				insert(opn1, -1);
				//opn2�� opn1_frcP-opn2_frcP insert;
			} else {
				insert(opn1, opn1_frcP - opn2_frcP - 1);
				insert(opn2, -1);
				//opn1�� opn1_frcP-opn2_frcP insert;
			}
			if(val == '+') {
				//printf("%c\n", opn1->Operator->val);
				calADD(opn1, opn2);
				// opn1 + opn2;
				// ����� �� ���ÿ� �ٽ� �־��ָ� ��
				ans = opn2;
				push(stack, opn2);
				now = now->next;
			} else {
				calSUB(opn2, opn1);
				// pop2 - pop1;
				ans = opn2;
				push(stack, opn2);
				now = now->next;
				// ���� �Լ� ����
			}
			// ���ÿ��� 2���� �ǿ����ڸ� ���� ����ؾ� �Ѵ�.
		} else {
			push(stack, now);
			now = now->next;
			// �� �����ڴ� ���ÿ� �־�д�.
		}
	}
	printf("Final Answer is ");
	OPNodePrint(ans);
}
void calADD(OPN* opn1, OPN* opn2) {
	// opn2 + opn1 ���·� ����. 
	if (opn2->negative == 0 && opn1->negative == 1) {
		//  ��� + ���� --> ��� - ���  
		opn1->negative = 0;
		calSUB(opn2, opn1); // ��� opn2 - ��� opn1 
		return;
	} else if (opn2->negative == 1 && opn1->negative == 0) {
		// ���� opn2 + ��� opn1 --> ��� opn1 - ��� opn2; 
		opn2->negative = 0;
		calSUB(opn1, opn2);
		return;
	} else {
		int intP = opn1->intPart;
		int frcP = opn1->frcPart;
		// �ڸ���
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
}
void calSUB(OPN* opn2, OPN* opn1) {
	//opn2 - opn1
	int intP = opn2->intPart;
	int frcP = opn2->frcPart;
	int i, num1, num2, num3;
	Node *now1;
	Node *now2;
	if (opn1->negative) {
		// opn2 - (-opn1)�� ���� 
		opn1->negative = 0;
		calADD(opn1, opn2); //opn2 + opn1;
	}
	now1 = opn1->Operator;
	now2 = opn2->Operator;
	int upcount = 0;
	//�Ҽ� -> ������ �Ѿ�� �ѱ�� ����
	int nextcount = 0;
	for (i=0; i<frcP; i++) {
		//�Ҽ��� ���
		num2 = (int)(now2->val) - 48;
		//chr -> int ����
		num1 = (int)(now1->val) - 48;
		num3 = num2 - num1 + nextcount;
		//���� ����
		nextcount = 0;
		//�ʱ�ȭ 
		if(num3 < 0) {
			//0���� ����
			Node *horse = now2->prev;
			//���ڸ� horse
			now2->val = (num3 + 10) + 48;
			//���� �����
			if((char)horse->val == '.') {
				//���ڸ��� "."�̸� (���� �ڸ��� �Ҽ� ù°�ڸ� ���)
				upcount = -1;
				//���ڸ��� -1�� �Ѱ���
			} else {
				//ù°�ڸ��� �ƴ�
				horse->val -= 1;
				//���ڸ� -1
			}
		} else {
			//0���� ŭ
			now2->val = num3 + 48;
			//�״�� ����
		}
		now2 = now2->prev;
		//�����ڸ��� �̵�
		now1 = now1->prev;
	}
	if(now2->val == '.') {
		// ���� "."�� index �Ǿ��ִٸ� �����η� �̵�
		now2 = now2->prev;
	}
	if(now1->val == '.') {
		now1 = now1->prev;
	}
	//printf("%c %c", now1->val, now2->val);
	for (i=0; i<intP; i++) {
		//������ ��
		num2 = (int)(now2->val) - 48;
		//chr -> int ����
		num1 = (int)(now1->val) - 48;
		num3 = num2 - num1 + nextcount;
		if(i == intP-2) {
			//�Ǿ��ڸ�
			if(num3 < 0) {
				//��ü ���� ������, �� ������ ���
				opn2->negative = 1;
				//������ �ٲ�
				now2->val = num3*(-1) + 48;
				if (nextcount == -1) {
					now2->val -= 1;
				}
				now2 = now2->next;
			} else {
				//��ü ���� ������ �ƴ�
				now2->val = num3 + 48;
			}
			break;
		} else {
			//�� ���ڸ� �ƴ�
			nextcount = 0;
			if (num3 < 0) {
				//������
				now2->val = 48 + 10 + num3;
				nextcount = -1;
			} else {
				//������ �ƴ�
				now2->val = num3 + 48;
				//�״�� ����
			}
		}
		now2 = now2->prev;
		now1 = now1->prev;
	}
}
void main() {
	DLL *list = newDLL();
	// ���ڿ� (��ü ����) �Է��� ���� ����Ʈ
	FILE *input = fopen("input.txt", "r");
	char *str;
	int cnt[2] = {
		0, 0
	}
	;
	int t = 1;
	//���� �ڸ���, �Ҽ� �ڸ��� üũ�뵵
	int ngtCheck = 0;
	// ���� ó�� �뵵
	int firstngt = 0;
	// �� ó���� ���� �����ڰ� ���°�� ó�� 
	OP *operand = newOP();
	// ���ڸ��� �����ϱ����� OP
    while (fscanf(input, "%c", &str) != EOF) {
		if ((char)str != '=') {
			if ((char)str == '.') {
				// str�� '.'�� ��
				if(t!=1) {
					printf("�Ҽ� �Է� ����. \n�߸� �Էµ� ������ ���� ���õ˴ϴ�.\n");
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
						printf("������ �Է� ����\n");
						break;
					}
				}
				t = 1;
				Node *now = list->head;
				while(now->next != NULL) {
					// now�����͸� list�� �ǳ����� �̵�
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
				// OP�� ���� �߰� �� cnt �ʱ�ȭ
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
						printf("������ �Է� ����\n");
						break;
					}
					t = 1;
					Node *now = list->head;
					while(now->next != NULL) {
						// now�����͸� list�� �ǳ����� �̵�
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
						// OP�� ���� �߰� �� cnt �ʱ�ȭ
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
	//printf("======[1������]���� ǥ�� ���� ======\n");
	//Oprint(operand);
	operand = in_to_postfix(operand);
	//printf("\n\n======[2������]���� ǥ�� ��ȯ======\n");
	//Oprint(operand);
	calculate(operand);
	//print(list);
}
