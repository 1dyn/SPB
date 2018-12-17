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
	//���� ������ ��� ex) 1.0 
	while(k > 0) {
		temp = temp->prev;
		k--;
	}
	k = intP + frcP;
	int temp1 = 0;
	while(k > 0) {
		if (MTPnum > 1) {//2�� �̻� ����
			Node *curNode = temp;
			for(int n=0; n < MTPnum-1; n++) {
				curNode = curNode->next;
			}
			if (curNode->val == '.') {
				break;
			}
		}
		if(temp->val == '0' && temp1==0) {
			// 030.3000 ó�� �տ� 0�� �ִ� ��� print ����
		}
		else if(temp->val =='.') {
			temp= temp->next;
			k--;
			break;
			// .�� ���� ��� �ϴ� print �ߴ�.
		}
		else {
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
		if(temp->val == '+' || temp->val == '-' || temp->val == '*'|| temp->val == '('|| temp->val == ')') {
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
		for (ii=0;ii<temp1;ii++) {
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
		} else if (val == '+' || val == '-'|| val == '*') {
			// (Pop�Ͽ� �����ȣ�� ��� list�� ����, �ƴҰ�� �ٽ� PUSH) & ���� �����ȣ PUSH
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
				if (opn2->negative == 0 && opn1->negative == 0) {
					//��� + ���
					calADD(opn2,opn1);
					ans = opn1;
				} else if (opn2->negative == 0 && opn1->negative == 1) {
					//��� + ����
					opn1->negative = 0;
					calSUB(opn2,opn1);
					ans = opn2;
				} else if (opn2->negative == 1 && opn1->negative == 0) {
					//���� + ���
					opn2->negative = 0;
					calSUB(opn1,opn2);
					ans = opn1;
				} else {
					//���� + ����
					calADD(opn2,opn1);
					opn2->negative = 1;
					ans = opn2;
				}
				push(stack, ans);
				now = now->next;
			} else if (val == '-'){
				if (opn2->negative == 0 && opn1->negative == 0) {
					//��� - ���
					calSUB(opn2,opn1);
					ans = opn2;
				} else if (opn2->negative == 0 && opn1->negative == 1) {
					//��� - ����
					opn1->negative = 0;
					calADD(opn2,opn1);
					ans = opn2;
				} else if (opn2->negative == 1 && opn1->negative == 0) {
					//���� - ���
					calADD(opn2,opn1);
					opn2->negative = 1;
					ans = opn2;
				} else {
					//���� - ����
					calSUB(opn1,opn2);
					opn1->negative = 1;
					ans = opn1;
				}
				push(stack, ans);
				now = now->next;
			} else {
				// ���ϱ��� ���...
				calMTP(opn1, opn2);
				ans = opn2;
				push(stack, ans);
				now = now->next;
			}
			// ���ÿ��� 2���� �ǿ����ڸ� ���� ����ؾ� �Ѵ�.
		} else {
			push(stack, now);
			now = now->next;
			// �� �����ڴ� ���ÿ� �־�д�.
		}
	}
	printf("Final Answer is ");
	if (ans->negative == 1) {
		printf("-");
	}
	OPNodePrint(ans);
}

void calADD(OPN* opn1, OPN* opn2) {
	// opn2 + opn1 ?�태�??�어?? 
	int intP = opn1->intPart;
	int frcP = opn1->frcPart;
	// ?�릿??
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
				// �� ù�ڸ� �� ���� ������ ���� ���
				opn2->negative = 1;
				calSUB(opn1, opn2);
				// �ϴ� �ٲ������ ���. 
				return;
			} else if (now2->prev->val == '0'){
				// ���� �մ� ��� 
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
	// ���� ó���� ������ ������ �� ����.
	/* ��� ����
	  1. �� ������ �� ���� 0�� .�� ������ ���ο� ��带 ����� �д�.
	  2. �� �κ� ���� ������ �����ϸ�, �� �ڸ����� �����ִ� ���·� �����Ѵ�. 
	  3. ���� ������ �����ϸ鼭 ���� ����� ���ο� Node�� �ʿ��ϴ�.
	  4. ������ ������ �Ҽ� �ڸ����� ���߾� .�� �־��ִ� ������� �����Ѵ�. 
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
		//trimOPN(DLL)�� �߰����ش�.
		//�� TRIM�� ��������. ���⼭ �Ҽ����� �Ű澲�� ����. ������ �˾Ƽ� �ȴ�. 
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
	// trim �۾��� ���� �Ҽ� �ڸ����� ��DLL���� �������.
	// ���� trimNode���� �� �ڸ����� �ϴ� �ο� ��ġ��.
	// opn1 * opn2 �̹Ƿ�, opn2�� �ڸ��� �̵����� ������ �����ϵ��� ��.
	// �׸��� �������ϱ� trimNode1, trimNode2�� �����Ϸ� ������ �ϴ� ���� ����. 
	
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
	// ��� ������� �� �� ģ����...

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
					//������ �տ� 0�� ä�� �ݴϴ�. 
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
		//trimtemp�� ���� �����ϴ� ���� +  
		// �ϳ��� �ڸ��� ������ ��� ����� ��Ȳ 
		
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
		// Ȥ�� �ʰ��ߴٸ�.. �÷�����. 
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
		// �ڸ����� �����ֱ� ���� ��ġ�� �����ϴ� �۾�.
		// trimtemp DLL���� �� �ڸ��� ����� ���� ����Ǿ� ����
		// ���� �ڸ����� �ݿ��� trimtemp�� ���� trimResultNode�� ���ϴ� ������ ����
		
				
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
		// temp + result ������ �����մϴ�.
		
		while(TRN->next != NULL){
			TRN = TRN->next;
		}
		while(TTN->next != NULL){
			TTN->val = '0';
			TTN = TTN->next;
		}
		// ���� ������ ����ߴ� �ֵ��� �ǵ��� ����. 
		
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
	// ���� TRN�� �� �������� ������ �ϰ� ����. 00060 �̷�����.
	// ���� �Ҽ��� ����غ��ô�...
	// �Ҽ��� ��굵 �� �̻��Ѱ� ����... �̤�  
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
	// �� �κе� ������ �ֽ��ϴ�. 
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
	// ��Ȱ�� ���� ����;; 
	
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
			} else if ((char)str == '*') {
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
	//printf("�Է� �Ϸ� \n");
	operand = in_to_postfix(operand);
	//printf("���� ���� �Ϸ� \n");
	//printf("\n\n======[2������]���� ǥ�� ��ȯ======\n");
	//Oprint(operand);
	calculate(operand);
	//printf("��� �Ϸ� \n");
	//print(list);
}
