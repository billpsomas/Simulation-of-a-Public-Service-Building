#include <iostream>
#include <cstdlib>
#define FLOORS 4
#define OFFICES 10

using namespace std;

class Visitor {   
	int priority; //� ������� �������������� ���� ���������
	int floor; //� ������ ��� ������ �� ���������� � ����������
	int office; //�� ������� ��� ������ �� ���������� � ����������
	Visitor* next;
public:
	Visitor(int priority_num, Visitor* ptr=NULL) : priority(priority_num), next(ptr) { 
		office = rand()%OFFICES+1; //������ ���� ��� 1-10 ��� �� office
	    floor = rand()%FLOORS+1; //������ ���� ��� 1-4 ��� �� floor
		cout<<"Visitor "<<priority<<" has been created: "<<"Floor "<<floor<<" and office "<<office<<endl;
	}
	int priority_get() {return priority;} //���������� ��� �������������
	Visitor* next_get() {return next;}
	void set_next(Visitor* visitor) {next = visitor;}
	void print() {cout<<"Visitor "<<priority<<endl;}
	int floor_get() {return floor;} //���������� ��� �����
	int office_get() {return office;} //����������  �� �������
};

class Office {   
	int No;
	int num_of_vis;
	int office_number;
	Visitor* exit_visitors;
public:
	Office(int no, int num, int vis=0, Visitor* ptr=NULL) : No(no), num_of_vis(vis), office_number(num) , exit_visitors(ptr) {
		cout<<"Office "<<office_number<<" has been created!"<<endl;
	}
	~Office() {cout<<"End of the work"<<endl;}
	void enter(Visitor *visitor);
	Visitor* exit();
	void print();
	int space() {return (No-num_of_vis);}
};

class Floor {   
	int Nf;
	int floor_number;
	int num_of_vis;
	Office** offices;
	Visitor* entrance;
	Visitor* exit_room;
public:
	Floor(int no, int nf, int floor, int vis=0, Visitor* ptr=NULL, Visitor* ptr1=NULL ) : Nf(nf), floor_number(floor), num_of_vis(vis), entrance(ptr), exit_room(ptr1) {
		offices = new Office*[OFFICES];
	    for(int i=0; i<OFFICES; i++) { offices[i] = new Office(no,i+1);}
	    cout<<"Floor "<<floor_number<<" has been created!\n "<<endl;
	}
	~Floor() {
		for(int i=0; i<OFFICES; i++)
			delete offices[i];
			delete[] offices;
			cout<<"End of waiting people"<<endl;
	}
	int enter(Visitor* visitor);
	Visitor* exit();
	int space() {return (Nf-num_of_vis);}
	int floor_get() {return floor_number;}
	void print();
	Visitor* choose_visitor(int office);
	Visitor* entrance_exit(int office);
	void enter_office(Visitor* visitor);
	int exit_room_enter(Visitor* visitor);
	Visitor* exit_room_exit();
	Visitor* exit_get() {return exit_room;}
	void exit_print();
};

class Ground_floor {  
	int Ng;
	int num_of_vis;
	Visitor* entrance;
public:
	Ground_floor(int ngr, int vis=0, Visitor* ptr=NULL) : Ng(ngr), num_of_vis(vis), entrance(ptr) {   
	cout<<"\nGround floor has been created\n"<<endl;}
	~Ground_floor()	{cout<<"End of service"<<endl;}
	int enter(Visitor* visitor);
	Visitor* exit_visitors();
	void entrance_insert(Visitor* visitor);
	void print();
	int visitors_get() {return num_of_vis;}
};

class Lift {	
	int Nl;
	int num_of_vis;
	Visitor* visitors_up;
	Visitor* visitors_down;
public:
	Lift(int nl, int vis=0, Visitor* ptr1=NULL, Visitor* ptr2=NULL) : Nl(nl), num_of_vis(vis), visitors_up(ptr1), visitors_down(ptr2) {
	cout<<"Lift has been created\n"<<endl;}
	~Lift() {cout<<"End of ups and downs"<<endl;}
	int space_get() {return (Nl-num_of_vis);}
	void up_enter(Visitor* visitor);
	Visitor* choose_visitor(int floor);
	Visitor* up_exit(int floor);
	void down_enter(Visitor* visitor);
	void up_print();
	void down_print();
	void empty_all();
};

class Building {
	int N;
	int num_of_vis;
	Ground_floor ground_floor;
	Lift lift;
	Floor** floors;
public:
	Building(int n, int ng, int nl, int no, int nf, int vis=0) : N(n), num_of_vis(vis), ground_floor(ng), lift(nl) {
		floors = new Floor*[FLOORS];
		for(int i=0; i<FLOORS; i++)
		    floors[i] = new Floor(no,nf,i+1);
		cout<<"A new building is ready for serving citizens!\n"<<endl;
	}
	~Building() {   
		for(int i=0; i<FLOORS; i++)
			delete floors[i];
		delete[] floors;
		cout<<"Service not available any longer. Go elsewhere\n"<<endl; 
	}
	void enter(Visitor** visitors, int K);
	void stop_up();
	void stop_down();
	void operate();
};
int Floor::enter(Visitor* visitor) {
	if (visitor!=NULL) {
		cout<<"Visitor "<<visitor->priority_get()<<" entered floor "<<floor_number<<endl;
		if (offices[visitor->office_get()-1]->space()) //�� ������� ���������� ����� ��� ������� ��� ����� �� ���� � ����������
			offices[visitor->office_get()-1]->enter(visitor);
		else {	
			cout<<"No space in office "<<visitor->office_get()<<".Visitor "<<visitor->priority_get()<<" please wait at the entrance."<<endl;
		    num_of_vis++;
			if (entrance == NULL) //����� ����� - �������� ��� ��������� ���� ����
			   entrance = visitor;
			else {
				Visitor* temp = entrance;
		    	while (temp->next_get() != NULL)
					temp = temp->next_get(); //������� ��� ��������� �����
				temp->set_next(visitor); //�������� ��� ��������� ��� ����� ��� ������ - � �������� ��� ��������� ������� ��� �� NULL
			}
		}
		return 1;
	} 
	else return 0; //��� ������� ����� ���������� ��� ����� visitors_up ��� lift ��� �� ����� �� ���������� ��� �����
}

void Floor::print() { //��������� ���� ���������� ������� ��� ������
	cout<<"\nFloor "<<floor_number<<" entrance has now "<<num_of_vis<<" visitors: "<<endl;
	Visitor* temp = entrance;
	while (temp!=NULL) {
		temp->print(); //��������� ��� ���������
		temp = temp->next_get();
	}
	cout << '\n';
}

void Floor::exit_print() { //��������� ���� ���������� ������� ��� ������
	cout << "\nExit room's visitors :" << endl;
	Visitor* temp = exit_room;
	while(temp!=NULL) {
		temp->print(); //��������� ��� ���������
		temp = temp->next_get();
	}
	cout << "\n";
}

int Floor::exit_room_enter(Visitor* visitor) {
	if (visitor!=NULL) {
		if (exit_room == NULL) //����� ����� - �������� ��� ��������� ���� ����
		   exit_room = visitor;
		else {
			Visitor* temp = exit_room;
			while (temp->next_get() != NULL)
				temp = temp->next_get(); //������� ��� ��������� �����
			temp->set_next(visitor); //�������� ��� ��������� ��� ����� ��� ������ - � �������� ��� ��������� ������� ��� �� NULL
		}
//	cout << "Visitor " << visitor->priority_get() << " goes to the exit room!" << endl;
	return 1;
	}
	else return 0; //���������� 0 �� �� �������� ����� ���������� ��� ��������� �� ��� �����
}

Visitor* Floor::exit() {
	Visitor* temp;
	for (int i=0; i<OFFICES; i++) { //������ ���� ��������� ��������� ��� ������� 1-10
		temp = offices[i]->exit();
     	if (temp!=NULL)
        	break;
	}
	return temp;
}

void Floor::enter_office(Visitor* visitor) {
	int v_office = visitor->office_get(); 
	offices[v_office-1]->enter(visitor); //������� ��� ��������� ��� ������� - �� ���������� ������� ������������� ���� ����� ���� ������� ����� ��� �� �������
}

Visitor* Floor::choose_visitor(int office) { //���������� NULL �� ��� ������� ���������� �� ���� ��� �������
	if (entrance != NULL) //�� � ����� ��� ����� �����
	   return entrance_exit(office);
	else return NULL;
}

Visitor* Floor::entrance_exit(int office) { //�������� ��� ��������� ��� ����� �� ���� ��� �������, ��� ������ ��� �� ����� ��� ��� ����������
	Visitor* temp = entrance;
	Visitor* temp_prev;
	if (temp->office_get()==office) { //�� � ������ ���������� ����� �� ���� ��� �������
		entrance = temp->next_get();
		num_of_vis--;
		temp->set_next(NULL);
		return temp;
	}
	while(temp->office_get()!=office) {
		if (temp->next_get() == NULL)
	    	return NULL; //�� ������� ������� ���������� ��� �����
	    temp_prev = temp;
		temp = temp->next_get();
	}
	temp_prev->set_next(temp->next_get());
	num_of_vis--;
	temp->set_next(NULL);
	return temp;
}

Visitor* Floor::exit_room_exit() { //������� ��� ��� ����
	if(exit_room!=NULL) { //�� � ����� ��� ����� ����
		Visitor* temp = exit_room;
		exit_room = temp -> next_get(); //� ������� ������� ���� ������� ���������
		temp -> set_next(NULL); //����� ��� ������� ��� ��������� ��� ������ ��� �� ������ �� ������� �� NULL
		return temp;
	}
	return NULL;
}

void Lift::up_print() { //��������� ���� ���������� ������� ��� ������
	cout<<"\nLift has now "<<num_of_vis<<" visitors"<<endl;
	Visitor* temp = visitors_up;
	while(temp!=NULL) {
		temp->print(); //�������� ��� ���������
		temp = temp->next_get();
	}
	cout << "\n";
}

void Lift::down_print() { //��������� ���� ���������� ������� ��� ������
	cout << "\nLift has now  " << num_of_vis << " visitors heading to the exit" <<endl;
	Visitor* temp = visitors_down;
	while(temp!=NULL) {
		temp->print(); //��������� ��� ���������
		temp = temp->next_get();
	}
	cout << "\n";
}

void Lift::up_enter(Visitor* visitor) {
	if (visitor!=NULL) {
		num_of_vis++;
		if (visitors_up == NULL) //����� ����� - �������� ��� ��������� ���� ����
		   visitors_up = visitor;
		else {
			Visitor* temp = visitors_up;
			while (temp->next_get() != NULL)
				temp = temp->next_get(); //������� ��� ��������� �����
			temp->set_next(visitor); //�������� ��� ��������� ��� ����� ��� ������
		}
	cout<<"Visitor "<<visitor->priority_get()<<" in the lift!"<<endl;
	}
}

void Lift::down_enter(Visitor* visitor) {
	if (visitor!=NULL) {
		num_of_vis++;
		if (visitors_down == NULL) //����� ����� - �������� ��� ��������� ���� ����
		   visitors_down = visitor;
		else {
			Visitor* temp = visitors_down;
			while (temp->next_get() != NULL)
				temp = temp->next_get(); //������� ��� ��������� �����
			temp->set_next(visitor); //�������� ��� ��������� ��� ����� ��� ������
		}
	cout<<"Visitor "<<visitor->priority_get()<<" returns to the lift!"<<endl;
	}
}

Visitor* Lift::choose_visitor(int floor) {      
	if (visitors_up != NULL) //�� � ����� visitors_up ��� ����� �����
	   return up_exit(floor);
	else return NULL; //���������� NULL �� ��� ������� ���������� �� ���� ���� �����
}

Visitor* Lift::up_exit(int floor) { //�������� ��� ��������� ��� ����� �� ���������� ��� �����, ��� ������ ��� �� ����� ��� ��� ����������
	Visitor* temp=visitors_up;
	Visitor* temp_prev;
	if (temp->floor_get()==floor) { //�� � ������ ���������� ����� �� ���� ���� �����
		visitors_up=temp->next_get();
		num_of_vis--;
		temp->set_next(NULL);
		return temp;
	}
	while(temp->floor_get()!=floor) {
	if (temp->next_get()==NULL)
	        return NULL; //�� ������� ������� ���������� ��� �����
	    temp_prev=temp;
		temp=temp->next_get();
	}
	temp_prev->set_next(temp->next_get());
	num_of_vis--;
	temp->set_next(NULL);
	return temp;
}

void Lift::empty_all() {
	while(visitors_down != NULL) {
		cout<<"Visitor "<<visitors_down->priority_get()<<" I can't believe i finished!\n";
		visitors_down=visitors_down->next_get();
		num_of_vis--;
	}
}

void Building::enter(Visitor** visitors, int K) { //�������� ��� K ���������� ��� �������
	for(int i=0; i<K; i++) {	
	if (ground_floor.enter(visitors[i])) //���������� ��� ����� ���� ����� ������� � entrance ��� ��������
		num_of_vis++; //������� ��� ������ � ������� ��� ������� ���� �������� ���������
	}
	ground_floor.print();
}

void Building::operate() { //�������� ���������� ��� lift ��� ��� ������� ����� ��� lift ��� �������� ���������� ��� �������
	while(lift.space_get() && ground_floor.visitors_get()!=0) //�������� ���������� ��� lift ��� ��� ������� ����� ��� lift ��� �������� ���������� ��� �������
	   lift.up_enter(ground_floor.exit_visitors());
	ground_floor.print();
	lift.up_print();
	cout << "!!! STOP UP !!!\n\n";
	stop_up();
	cout << "\n!!! STOP DOWN!!!\n";
	stop_down();
	lift.empty_all(); //� ���� visits_down ��������
}

void Building::stop_up() {
	for (int i=0; i<FLOORS; i++) {
		int check;
		cout << "\n\nFLOOR " << i+1 << ":\n" << endl;
		while (floors[i]->space()) { //�� ������� ���������� ����� ���� ����� i
			check = floors[i]->enter(lift.choose_visitor(i+1));
			if(check==0) //�� ��� �������� ���������� �� ��������� ��� ����� i, ������� � ����������
		    break; 
		}
//floors[i]->print();
//lift.up_print();
	}
}

void Building::stop_down() {
	int check;
	Visitor* temp;
	Visitor* ptr;
	for (int i=FLOORS-1; i>=0; i--) {
		cout << "\n\nFLOOR " << i+1 << ":\n" << endl;
		while (lift.space_get()) { //�� ������� ���������� ����� ��� lift
			temp = floors[i]->exit(); //��� temp ��������� � ���������� ��� ����� ��� �� �������
			check = floors[i]->exit_room_enter(temp); //� temp ��������� ��� ���� ������ ��� ������
 			if (check==0) //�� �� check ����� �����, ��� �������� ����� ���������� ��� ��������� ���� �����
		        break;
		}
	    floors[i]->print();
	   	floors[i]->exit_print(); //����� � ������� ��� ���������� ��� �� �������
		//�������� ���������� �� ����� ��� ��� ����� ����� ������ ����������
		temp = floors[i]->exit_get(); //������� ���� ���� exit_room
		while(temp!=NULL) {
			ptr = floors[i]->choose_visitor(temp->office_get()); //�� ��������� ��� ��� ������� ���������� ��� ������� ���������� NULL
			if (ptr != NULL) //H choose_visitor ��������� ��� �� ����� ��� entrance ��� ���������, ����� ��� ����������� ��������� ��� �� ������, ��� ����������
			   floors[i]->enter_office(ptr); //� ���������� ��������� ��� �������
			temp = temp->next_get();
		}
 		temp = floors[i]->exit_get();
	    while(temp!=NULL) { //�������� ��� ���������� ��� exit_room ��� lift
			temp = floors[i]->exit_room_exit();
	        if (temp!=NULL)
			   lift.down_enter(temp); //������ ��� ����� ��� ������ ��� ��� ���������� ���� enter ��� ��� ������� ��� visitors_down ��� lift
		} 
	}
	lift.down_print();
}

int Ground_floor::enter(Visitor* visitor) {
	if (num_of_vis < Ng) { //�� �� ������� ��� ���� �������   
		num_of_vis++;
		entrance_insert(visitor);
		cout<<"Visitor "<<visitor->priority_get()<<" is waiting for the lift"<<endl;
		return 1;
	}
	else {
		cout<<"Please,come tomorrow visitor "<<visitor->priority_get()<<endl;
		return 0;   
	}
}

void Ground_floor::entrance_insert(Visitor* visitor) {   
	if(entrance==NULL) //�� � ����� ����� �����, ���� �� ���� ��������� ���� ���� � ����������
	   entrance=visitor;
	else {
		Visitor* temp = entrance;
		while (temp->next_get() != NULL)
			temp = temp->next_get(); //������� ��� ��������� �����
		temp->set_next(visitor);
	}
}

void Ground_floor::print() { //��������� ���� ���������� ������� ��� ������
	cout<<"\nGround_floor entrance has now "<<num_of_vis<<" visitors\n"<<endl;
	Visitor* temp = entrance;
	for (int i=0; i<num_of_vis; i++) {
		temp->print(); //�������� ���������
		temp = temp->next_get();
	}
	cout << '\n';
}

Visitor* Ground_floor::exit_visitors() {
	num_of_vis--;
	if(entrance==NULL) {      //�� � ����� ��� ������� ����� ����
		cout<<"There are no more visitors waiting for the lift.\n"<<endl;
	    return NULL;
	}
	else {
		Visitor* temp = entrance;
	    entrance = temp -> next_get();      //� ������� entrance ������� ���� ������� ���������
		temp -> set_next(NULL);             //����� ��� ������� ��� ��� ��������� ��� ������ ��� �� ������� �� ������� �� NULL
		return temp;
	}
}

void Office::enter(Visitor* visitor) {
	if(exit_visitors == NULL) //�� � ����� ����� �����, ���� � ���������� ������� ���� ���� ���
	   exit_visitors = visitor;
	else {
	Visitor* temp = exit_visitors;
	   while (temp->next_get() != NULL)
			temp = temp->next_get(); //������� ��� ��������� �����
		temp->set_next(visitor);
	}
	num_of_vis++;
	cout<<"Visitor "<<visitor->priority_get()<<" entered office "<<office_number<<endl;
}

Visitor* Office::exit() {
	if(exit_visitors==NULL)
	    return NULL;
	else {
		Visitor* temp = exit_visitors;
	    exit_visitors = temp -> next_get(); //� ������� exit_visitors ������� ���� ������� ���������
		temp -> set_next(NULL); //����� ��� ������� ��� ��������� ��� ������ ��� �� ������ �� ������� �� NULL
		num_of_vis--;
		return temp;
	}
}

void Office::print() { //��������� ���� ���������� ������� ��� ������
	cout<<"\nOffice "<<office_number<<" has now "<<num_of_vis<<" visitors :"<<endl;
	Visitor* temp = exit_visitors;
	while (temp!=NULL) {
		temp->print(); //��������� ��� ���������
		temp = temp->next_get();
	}
	cout << '\n';
}

int main() {
	int N, Nf, Ng, No, Nl, K, L;
	Visitor** visitors;
	cout<<"Give me the maximum number of visitors for the building: ";
	cin>>N;
	cout<<"Give me the maximum number of visitors for each floor: ";
	cin>>Nf;
	while (Nf>N/3) {
		cout<<"Give me the correct maximum number of visitors for each floor: ";
		cin>>Nf;
	}
	cout<<"Give me the maximum number of visitors for the ground floor: ";
	cin>>Ng;
	while (Ng>N/2) {
		cout<<"Give me the correct maximum number of visitors for the ground floor: ";
		cin>>Ng;
	}
	cout<<"Give me the maximum number of visitors for each office: ";
	cin>>No;
	while (No>Nf) {
		cout<<"Give me the correct maximum number of visitors for each office: ";
		cin>>No;
	}
	cout<<"Give me the maximum number of visitors for the lift: ";
	cin>>Nl;
	while (Nl<No) {
		cout<<"Give me the correct maximum number of visitors for the lift: ";
		cin>>Nl;
	}
	cout<<"Give me the number of visitors: ";
	cin>>K;
	cout<<"Give me the number of operations of the lift: ";
	cin>>L;
	Building building(N,Ng,Nl,No,Nf); //���������� ��� ������� ��� ������������ ���� ��� ������������
	visitors = new Visitor*[K]; //���������� ��� ������������ ��� � ���������� 
	for (int i=0; i<K; i++)
	    visitors[i] = new Visitor(i+1);
	cout << "\n\n";
	building.enter(visitors,K); //������� ���� ������ ��� ������� ����� ���������� ������
	for (int i=0; i<L; i++) {
		cout << "\n -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-\n";
		cout << "\n\n LIFT OPERATION NUMBER : " << i+1 << " !!!\n\n " << endl;
		building.operate();
	}
	for (int i=0; i<K; i++) //����������� ������ ��� �� heap
	    delete visitors[i];
	delete[] visitors;
return 0;
}
