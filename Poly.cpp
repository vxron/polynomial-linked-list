#include "Poly.h"
#include <cmath>

// Default Constructor
Poly::Poly()
{
	// if no input is given, we want to default to a zero polynomial (deg = -1)
	// this is represented by an empty linked list (dummy header)
	head = new PolyNode(-1,0,NULL);
}

// Constructor from vectors
Poly::Poly(const std::vector<int>& deg, const std::vector<double>& coeff)
{
	// create dummy header
	head = new PolyNode(-1,0,NULL);

	// error handling
	if(deg.empty()||coeff.empty()){
		return;
	}

	// insert first element of list after header
	PolyNode* p = new PolyNode(deg[0],coeff[0],NULL);
	head->next = p;
	// for loop to add subsequent elements from vectors to nodes
	// adding most significant to least significant (sorted)
	for(unsigned int i=1;i<deg.size();i++){
		p->next = new PolyNode(deg[i],coeff[i],NULL);
		// move to next node
		p = p->next;
	}
}

// Destructor
Poly::~Poly()
{
	PolyNode* tmp = head;
	PolyNode* tnext;
	// do while so we run at least ONCE to free head
	do{
		tnext = tmp->next;
		delete tmp;
		tmp=tnext;
	}while(tnext!=NULL);
	// stop running once tmp pointer reaches NULL (indicating last node has been reached)
}

void Poly::addMono(int i, double c)
{
	// degree i, coeff c

	// iterate through nodes to check if degree matches anywhere
	// keep track of previous node to p to facilitate deletion
	int flag=0;
	PolyNode* p = head;
	while(p->next!=NULL){
		if(p->next->deg==i){
			flag=1;
			// this breaks at node right before node we want to delete or update
			break;
		}
		else if(p->next->deg<i){
			// this breaks at first node (p->next) to have degree smaller than i
			break;
		}
		p=p->next;
	}

	// case I: adding degree that's already there (updating preexistent node)
	// when we exit loop, if flag=1, p=node before one that needs to be updated
	if(flag==1){

		// subcase I: degree goes to 0 (deleting node)
		if(p->next->coeff+c==0){
			PolyNode* tmp=p->next->next; // node after node that needs to be deleted
			delete p->next; // delete
			p->next=tmp; // set pointer of p to new appropriate node
		}

		else{ // update node
			p->next->coeff+=c;
		}

	}

	// case II: adding new degree (inserting node)
	// when we exit loop, if flag=0, p=node we neeed to insert at (before p->next)
	else{
		PolyNode* tmpnext = p->next; // current element after p, must be shifted up one
		p->next = new PolyNode(i,c,tmpnext); // insert!
	}
}

void Poly::addPoly(const Poly& p)
{

	// split up into monomials and add sequentially by calling addMono
	// for each "node" of polynomial, get degree & coefficient
	// and add to THIS polynomial using addMono
	PolyNode* it = p.head;
	int tempdeg;
	double tempcoeff;
	while(it->next!=NULL){
		tempdeg=it->next->deg;
		tempcoeff=it->next->coeff;
		this->addMono(tempdeg,tempcoeff);
		it=it->next;
	}

}

void Poly::multiplyMono(int i, double c)
{

	// if monomial coeff is 0, this becomes empty
	if(c==0){

		PolyNode* tmp = this->head->next;
		PolyNode* tmpnext;
		while(tmp!=NULL){
			tmpnext = tmp->next;
			delete tmp;
			tmp=tmpnext;
		}
		head->next=NULL;
	}

	// if this is empty, it remains empty despite multiplication
	else if(this->head->next==NULL){
		return;
	}

	// every node in this gets multiplied by the monomial deg i, coeff c
	// no duplicate degrees -> so no adding like terms
	else{
		PolyNode* tmp=this->head;
		while(tmp->next!=NULL){
			tmp->next->deg+=i;
			tmp->next->coeff*=c;
			tmp=tmp->next;
		}
		// simply iterate through nodes
	}
}

// multiply p (input) to this
void Poly::multiplyPoly(const Poly& p)
{
	// split up into monomials and multiply sequentially by calling multiplyMono
	// for each "node" of polynomial, get degree & coefficient
	// and multiply to THIS polynomial using multiplyMono

	PolyNode* it = p.head->next;

	Poly original;
	Poly original2;

	// make duplicates of this poly to perform multiplication and reset
	this->duplicate(original); // original now contains this poly
	this->duplicate(original2); // original2 now contains this poly

	int tempdeg;
	double tempcoeff;

	// if this is empty, it remains empty despite multiplication
	if(this->head->next==NULL){
		return;
	}

	// if it's multiplication by 0, this all becomes 0
	if(p.head->next==NULL){
		// 0 multiplication
		this->multiplyMono(1,0);
		return;
	}

	// zero case for non-empty polynomials
	tempdeg=it->deg;
	tempcoeff=it->coeff;
	// initialize this to contain first distribution
	this->multiplyMono(tempdeg,tempcoeff);

	// each multiply mono gets its own linked list, then add together using addPoly
	while(it->next!=NULL){
		tempdeg=it->next->deg;
		tempcoeff=it->next->coeff;

		// get one distribution term of mono upon original THIS
		original.multiplyMono(tempdeg,tempcoeff);

		// add this distribution term to this total
		this->addPoly(original);
		it=it->next;

		// reset original to contain initial THIS, contained in original2
		original2.duplicate(original);
	}

}

void Poly::duplicate(Poly& outputPoly)
{
	PolyNode* op = outputPoly.head;

	// copy this poly to outputpoly

	PolyNode* p = this->head;

	// j keeps track of looping through current outputPoly (duplicate)

	int j=0;
	for(int i=0;i<(this->getTermsNo());i++){
		if(j<(outputPoly.getTermsNo())){
			// there are preexisting nodes we can simply update in outputPoly
			op->next->deg=p->next->deg;
			op->next->coeff=p->next->coeff;
		}
		else{
			// no more elements in outputPoly, need to create more
			op->next=new PolyNode(p->next->deg,p->next->coeff,NULL);
		}

		// increment
		p=p->next;
		op=op->next;
		j++;
	}

}

int Poly::getDegree() const
{
	int degree;
	// will be first node's degree (after head)
	if(head->next==NULL){
		degree=-1; // empty
	}
	else{
		degree = (head->next)->deg;
	}
	return degree;
}

int Poly::getTermsNo() const
{
	int terms=0;
	// loop through nodes until NULL pointer is reached
	PolyNode* tmp = head;
	while(tmp->next!=NULL){
		tmp=tmp->next;
		// counter
		terms++;
	}
	return terms;
}

double Poly::evaluate(double x)
{

	// iterate through nodes and add to psum coeff*x^deg
	double psum=0;
	double tmp=0;
	PolyNode* p= head;

	while(p->next!=NULL){
		p=p->next;
		tmp = (p->coeff)*pow(x,p->deg);
		//printf("%lf :) \n",tmp);
		psum+=tmp;
	}

	return psum;
}

std::string Poly::toString()
{
	std::string polynomial;
	PolyNode* p;
	// create temporary arrays for degrees and coefficients
	double* temp_coeff = new double [getTermsNo()];
	int* temp_deg = new int [getTermsNo()];

	int i=0;
	for(p=head->next;p->next!=NULL;p=p->next){
		temp_coeff[i]=p->coeff;
		temp_deg[i]=p->deg;
		i++;
	}

	//loop through list to print
	polynomial.append("degree=%d; ",getDegree());
	for(int j=0;j<i;j++){
		polynomial.append("a(%d)=%lf; ",temp_deg[j],temp_coeff[j]);
	}

	return polynomial;
}
