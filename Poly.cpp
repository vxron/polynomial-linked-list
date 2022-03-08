#include "Poly.h"

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
	// insert first element of list after header
	PolyNode* p = new PolyNode(deg[0],coeff[0],NULL);
	head->next = p;
	// for loop to add subsequent elements from vectors to nodes
	// adding most significant to least significant (sorted)
	for(unsigned int i=1;i<(deg.size()-1);i++){
		p->next = new PolyNode(deg[i],coeff[i],NULL);
		// move to next node
		p = p->next;
	}
}

// Copy Constructor

// Destructor
Poly::~Poly()
{
	PolyNode* tmp = head;
	PolyNode* tnext;
	// do while so we run at least ONCE to free head
	do{
		tnext = tmp->next;
		free(tmp);
		tmp=tnext;
	}while(tnext!=NULL);
	// stop running once tmp pointer reaches NULL (indicating last node has been reached)
}

void Poly::addMono(int i, double c)
{
	// TODO
}

void Poly::addPoly(const Poly& p)
{
	// TODO
}

void Poly::multiplyMono(int i, double c)
{
	// TODO
}

void Poly::multiplyPoly(const Poly& p)
{
	// TODO
}

void Poly::duplicate(Poly& outputPoly)
{
	// TODO
}

// done
int Poly::getDegree()
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

// done
int Poly::getTermsNo()
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
	// TODO
	return -1;//change this after completing this function
}

std::string Poly::toString()
{
	std::string polynomial;
	PolyNode* p;
	// create temporary arrays for degrees and coefficients
	double* temp_coeff = new double [getTermsNo()];
	int* temp_deg = new int [getTermsNo()];

	int i=0;
	for(p=head->next;p!=NULL;p=p->next){
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
