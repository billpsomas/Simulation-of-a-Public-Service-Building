#include <iostream>
#include <cstdlib>
#define FLOORS 4
#define OFFICES 10

using namespace std;

class Visitor {   
	int priority; //ο αριθμός προτεραιότητας κάθε επισκέπτη
	int floor; //ο όροφος που πρέπει να επισκεφτεί ο επισκέπτης
	int office; //το γραφείο που πρέπει να επισκεφτεί ο επισκέπτης
	Visitor* next;
public:
	Visitor(int priority_num, Visitor* ptr=NULL) : priority(priority_num), next(ptr) { 
		office = rand()%OFFICES+1; //τυχαία τιμή από 1-10 για το office
	    floor = rand()%FLOORS+1; //τυχαία τιμή από 1-4 για το floor
		cout<<"Visitor "<<priority<<" has been created: "<<"Floor "<<floor<<" and office "<<office<<endl;
	}
	int priority_get() {return priority;} //επιστρέφει την προτεραιότητα
	Visitor* next_get() {return next;}
	void set_next(Visitor* visitor) {next = visitor;}
	void print() {cout<<"Visitor "<<priority<<endl;}
	int floor_get() {return floor;} //επιστρέφει τον όροφο
	int office_get() {return office;} //επιστρέφει  το γραφείο
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
		if (offices[visitor->office_get()-1]->space()) //Αν υπάρχει διαθέσιμος χώρος στο γραφείο που θέλει να πάει ο επισκέπτης
			offices[visitor->office_get()-1]->enter(visitor);
		else {	
			cout<<"No space in office "<<visitor->office_get()<<".Visitor "<<visitor->priority_get()<<" please wait at the entrance."<<endl;
		    num_of_vis++;
			if (entrance == NULL) //’δεια λίστα - Εισαγωγή του επισκέπτη στην αρχή
			   entrance = visitor;
			else {
				Visitor* temp = entrance;
		    	while (temp->next_get() != NULL)
					temp = temp->next_get(); //Βρίσκει τον τελευταίο κόμβο
				temp->set_next(visitor); //Εισαγωγή του επισκέπτη στο τέλος της λίστας - ο επόμενος του επισκέπτη δείχνει ήδη σε NULL
			}
		}
		return 1;
	} 
	else return 0; //Δεν υπάρχει άλλος επισκέπτης στη λίστα visitors_up του lift που να θέλει να επισκεπτεί τον όροφο
}

void Floor::print() { //Εκτυπώνει τους υπάρχοντες κόμβους της λίστας
	cout<<"\nFloor "<<floor_number<<" entrance has now "<<num_of_vis<<" visitors: "<<endl;
	Visitor* temp = entrance;
	while (temp!=NULL) {
		temp->print(); //Εκτυπώνει τον επισκέπτη
		temp = temp->next_get();
	}
	cout << '\n';
}

void Floor::exit_print() { //Εκτυπώνει τους υπάρχοντες κόμβους της λίστας
	cout << "\nExit room's visitors :" << endl;
	Visitor* temp = exit_room;
	while(temp!=NULL) {
		temp->print(); //Εκτυπώνει τον επισκέπτη
		temp = temp->next_get();
	}
	cout << "\n";
}

int Floor::exit_room_enter(Visitor* visitor) {
	if (visitor!=NULL) {
		if (exit_room == NULL) //’δεια λίστα - Εισαγωγή του επισκέπτη στην αρχή
		   exit_room = visitor;
		else {
			Visitor* temp = exit_room;
			while (temp->next_get() != NULL)
				temp = temp->next_get(); //Βρίσκει τον τελευταίο κόμβο
			temp->set_next(visitor); //Εισαγωγή του επισκέπτη στο τέλος της λίστας - ο επόμενος του επισκέπτη δείχνει ήδη σε NULL
		}
//	cout << "Visitor " << visitor->priority_get() << " goes to the exit room!" << endl;
	return 1;
	}
	else return 0; //Επιστρέφει 0 αν εν υπάρχουν άλλοι επισκέπτες που τέλειωσαν με τον όροφο
}

Visitor* Floor::exit() {
	Visitor* temp;
	for (int i=0; i<OFFICES; i++) { //Εξάγει έναν επισκέπτη ψάχνοντας στα γραφεία 1-10
		temp = offices[i]->exit();
     	if (temp!=NULL)
        	break;
	}
	return temp;
}

void Floor::enter_office(Visitor* visitor) {
	int v_office = visitor->office_get(); 
	offices[v_office-1]->enter(visitor); //Εισάγει τον επισκέπτη στο γραφείο - δε χρειάζεται έλεγχος χωρητικότητας αφού μόλις πριν κάποιος βγήκε από το γραφείο
}

Visitor* Floor::choose_visitor(int office) { //Επιστρέφει NULL αν δεν υπάρχει επισκέπτης να μπει στο γραφείο
	if (entrance != NULL) //Αν η λίστα δεν είναι άδεια
	   return entrance_exit(office);
	else return NULL;
}

Visitor* Floor::entrance_exit(int office) { //Διαλέγει τον επισκέπτη που θέλει να μπει στο γραφείο, τον βγάζει από τη λίστα και τον επιστρέφει
	Visitor* temp = entrance;
	Visitor* temp_prev;
	if (temp->office_get()==office) { //Αν ο πρώτος επισκέπτης θέλει να μπει στο γραφείο
		entrance = temp->next_get();
		num_of_vis--;
		temp->set_next(NULL);
		return temp;
	}
	while(temp->office_get()!=office) {
		if (temp->next_get() == NULL)
	    	return NULL; //Δε βρέθηκε κανένας επισκέπτης στη λίστα
	    temp_prev = temp;
		temp = temp->next_get();
	}
	temp_prev->set_next(temp->next_get());
	num_of_vis--;
	temp->set_next(NULL);
	return temp;
}

Visitor* Floor::exit_room_exit() { //Εξαγωγή απο την αρχή
	if(exit_room!=NULL) { //Αν η λίστα δεν είναι κενή
		Visitor* temp = exit_room;
		exit_room = temp -> next_get(); //ο δείκτης δείχνει στον επόμενο επισκέπτη
		temp -> set_next(NULL); //Κάνει τον επόμενο του επισκέπτη που φεύγει από το κτίριο να δείχνει σε NULL
		return temp;
	}
	return NULL;
}

void Lift::up_print() { //Εκτυπώνει τους υπάρχοντες κόμβους της λίστας
	cout<<"\nLift has now "<<num_of_vis<<" visitors"<<endl;
	Visitor* temp = visitors_up;
	while(temp!=NULL) {
		temp->print(); //Εκτύπωση του επισκέπτη
		temp = temp->next_get();
	}
	cout << "\n";
}

void Lift::down_print() { //Εκτυπώνει τους υπάρχοντες κόμβους της λίστας
	cout << "\nLift has now  " << num_of_vis << " visitors heading to the exit" <<endl;
	Visitor* temp = visitors_down;
	while(temp!=NULL) {
		temp->print(); //Εκτυπώνει τον επισκέπτη
		temp = temp->next_get();
	}
	cout << "\n";
}

void Lift::up_enter(Visitor* visitor) {
	if (visitor!=NULL) {
		num_of_vis++;
		if (visitors_up == NULL) //’δεια λίστα - Εισαγωγή του επισκέπτη στην αρχή
		   visitors_up = visitor;
		else {
			Visitor* temp = visitors_up;
			while (temp->next_get() != NULL)
				temp = temp->next_get(); //Βρίσκει τον τελευταίο κόμβο
			temp->set_next(visitor); //Εισαγωγή του επισκέπτη στο τέλος της λίστας
		}
	cout<<"Visitor "<<visitor->priority_get()<<" in the lift!"<<endl;
	}
}

void Lift::down_enter(Visitor* visitor) {
	if (visitor!=NULL) {
		num_of_vis++;
		if (visitors_down == NULL) //’δεια λίστα - Εισαγωγή του επισκέπτη στην αρχή
		   visitors_down = visitor;
		else {
			Visitor* temp = visitors_down;
			while (temp->next_get() != NULL)
				temp = temp->next_get(); //Βρίσκει τον τελευταίο κόμβο
			temp->set_next(visitor); //Εισαγωγή του επισκέπτη στο τέλος της λίστας
		}
	cout<<"Visitor "<<visitor->priority_get()<<" returns to the lift!"<<endl;
	}
}

Visitor* Lift::choose_visitor(int floor) {      
	if (visitors_up != NULL) //Αν η λίστα visitors_up δεν είναι άδεια
	   return up_exit(floor);
	else return NULL; //Επιστρέφει NULL αν δεν υπάρχει επισκέπτης να μπει στον όροφο
}

Visitor* Lift::up_exit(int floor) { //Διαλέγει τον επισκέπτη που θέλει να επισκεπτεί τον όροφο, τον βγάζει από τη λίστα και τον επιστρέφει
	Visitor* temp=visitors_up;
	Visitor* temp_prev;
	if (temp->floor_get()==floor) { //Αν ο πρώτος επισκέπτης θέλει να μπει στον όροφο
		visitors_up=temp->next_get();
		num_of_vis--;
		temp->set_next(NULL);
		return temp;
	}
	while(temp->floor_get()!=floor) {
	if (temp->next_get()==NULL)
	        return NULL; //Δε βρέθηκε κανένας επισκέπτης στη λίστα
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

void Building::enter(Visitor** visitors, int K) { //Εισαγωγή των K επισκεπτών στο ισόγειο
	for(int i=0; i<K; i++) {	
	if (ground_floor.enter(visitors[i])) //Εισάγονται στη λίστα στην οποία δείχνει η entrance του ισογείου
		num_of_vis++; //Δίνεται σαν όρισμα ο δείκτης που δείχνει στον εκάστοτε επισκέπτη
	}
	ground_floor.print();
}

void Building::operate() { //Εισαγωγή επισκεπτών στο lift για όσο υπάρχει χώρος στο lift και υπάρχουν επισκέπτες στο ισόγειο
	while(lift.space_get() && ground_floor.visitors_get()!=0) //Εισαγωγή επισκεπτών στο lift για όσο υπάρχει χώρος στο lift και υπάρχουν επισκέπτες στο ισόγειο
	   lift.up_enter(ground_floor.exit_visitors());
	ground_floor.print();
	lift.up_print();
	cout << "!!! STOP UP !!!\n\n";
	stop_up();
	cout << "\n!!! STOP DOWN!!!\n";
	stop_down();
	lift.empty_all(); //Η ουρά visits_down αδειάζει
}

void Building::stop_up() {
	for (int i=0; i<FLOORS; i++) {
		int check;
		cout << "\n\nFLOOR " << i+1 << ":\n" << endl;
		while (floors[i]->space()) { //Αν υπάρχει διαθέσιμος χώρος στον όροφο i
			check = floors[i]->enter(lift.choose_visitor(i+1));
			if(check==0) //Αν δεν υπάρχουν επισκέπτες με προορισμό τον όροφο i, σταματά η διαδικασία
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
		while (lift.space_get()) { //Αν υπάρχει διαθέσιμος χώρος στο lift
			temp = floors[i]->exit(); //Στο temp κρατιέται ο επισκέπτης που έφυγε από το γραφείο
			check = floors[i]->exit_room_enter(temp); //Ο temp εισάγεται στο χώρο εξόδου του ορόφου
 			if (check==0) //Αν το check γίνει μηδέν, δεν υπάρχουν άλλοι επισκέπτες που τελείωσαν στον όροφο
		        break;
		}
	    floors[i]->print();
	   	floors[i]->exit_print(); //Εγινε η εξαγωγή των επισκεπτών απο τα γραφεία
		//Εισαγωγή επισκεπτών σε όροφο απο τον οποίο μόλις βγήκαν επισκέπτες
		temp = floors[i]->exit_get(); //Δείκτης στην ουρά exit_room
		while(temp!=NULL) {
			ptr = floors[i]->choose_visitor(temp->office_get()); //Σε περίπτωση που δεν υπάρχει επισκέπτης για γραφείο επιστρέφει NULL
			if (ptr != NULL) //H choose_visitor διαγράφει από τη λίστα της entrance τον επισκέπτη, κανει τις απαραίτητες συνδέσεις και αν βρεθεί, τον επιστρέφει
			   floors[i]->enter_office(ptr); //ο επισκέπτης εισάγεται στο γραφείο
			temp = temp->next_get();
		}
 		temp = floors[i]->exit_get();
	    while(temp!=NULL) { //Εισαγωγή των επισκεπτών της exit_room στο lift
			temp = floors[i]->exit_room_exit();
	        if (temp!=NULL)
			   lift.down_enter(temp); //Εξάγει τον πρώτο της λίστας και τον επιστρέφει στην enter που τον εισάγει στο visitors_down της lift
		} 
	}
	lift.down_print();
}

int Ground_floor::enter(Visitor* visitor) {
	if (num_of_vis < Ng) { //Αν το ισόγειο δεν έχει γεμίσει   
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
	if(entrance==NULL) //Αν η λίστα είναι άδεια, τότε σε αυτή εισάγεται στην αρχή ο επισκέπτης
	   entrance=visitor;
	else {
		Visitor* temp = entrance;
		while (temp->next_get() != NULL)
			temp = temp->next_get(); //Βρίσκει τον τελευταίο κόμβο
		temp->set_next(visitor);
	}
}

void Ground_floor::print() { //Εκτυπώνει τους υπάρχοντες κόμβους της λίστας
	cout<<"\nGround_floor entrance has now "<<num_of_vis<<" visitors\n"<<endl;
	Visitor* temp = entrance;
	for (int i=0; i<num_of_vis; i++) {
		temp->print(); //Εκτύπωση επισκέπτη
		temp = temp->next_get();
	}
	cout << '\n';
}

Visitor* Ground_floor::exit_visitors() {
	num_of_vis--;
	if(entrance==NULL) {      //Αν η λίστα της εισόδου είναι κενή
		cout<<"There are no more visitors waiting for the lift.\n"<<endl;
	    return NULL;
	}
	else {
		Visitor* temp = entrance;
	    entrance = temp -> next_get();      //Ο δείκτης entrance δείχνει στον επόμενο επισκέπτη
		temp -> set_next(NULL);             //Κάνει τον επόμενο από τον επισκέπτη που φεύγει από το ισόγειο να δείχνει σε NULL
		return temp;
	}
}

void Office::enter(Visitor* visitor) {
	if(exit_visitors == NULL) //Αν η λίστα είναι άδεια, τότε ο επισκέπτης μπαίνει στην αρχή της
	   exit_visitors = visitor;
	else {
	Visitor* temp = exit_visitors;
	   while (temp->next_get() != NULL)
			temp = temp->next_get(); //Βρίσκει τον τελευταίο κόμβο
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
	    exit_visitors = temp -> next_get(); //Ο δείκτης exit_visitors δείχνει στον επόμενο επισκέπτη
		temp -> set_next(NULL); //Κάνει τον επόμενο του επισκέπτη που φεύγει από το κτίριο να δείχνει σε NULL
		num_of_vis--;
		return temp;
	}
}

void Office::print() { //Εκτυπώνει τους υπάρχοντες κόμβους της λίστας
	cout<<"\nOffice "<<office_number<<" has now "<<num_of_vis<<" visitors :"<<endl;
	Visitor* temp = exit_visitors;
	while (temp!=NULL) {
		temp->print(); //Εκτυπώνει τον επισκέπτη
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
	Building building(N,Ng,Nl,No,Nf); //Δημιουργία του κτιρίου και αρχικοποίηση όλων των αντικειμένων
	visitors = new Visitor*[K]; //Δημιουργία και αρχικοποίηση των Κ επισκεπτών 
	for (int i=0; i<K; i++)
	    visitors[i] = new Visitor(i+1);
	cout << "\n\n";
	building.enter(visitors,K); //Εισάγει στην είσοδο του κτιρίου όσους επισκέπτες χωράνε
	for (int i=0; i<L; i++) {
		cout << "\n -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-\n";
		cout << "\n\n LIFT OPERATION NUMBER : " << i+1 << " !!!\n\n " << endl;
		building.operate();
	}
	for (int i=0; i<K; i++) //Αποδέσμευση μνήμης από το heap
	    delete visitors[i];
	delete[] visitors;
return 0;
}
