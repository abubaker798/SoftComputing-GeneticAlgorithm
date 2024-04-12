#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <map>
using namespace std;

string myText;
int numTestCases, sizKnapsack, numItems;
int maxGenerations = 3, change = 0;
int *chromosome , numOfChromosome = 4, *fitness;
int **items, **populations;
int firstSelChro, secondSelChro; // first and second select chromosome
// Read from the text file
ifstream knapsackFile;
ofstream outFile;
void insertItems();
bool isFitKnapsack( int ch[]);
void setPopulation();
int toDec(int bin[]);
int cumulativeFitness(int);
void setFitness();
void selection();
void crossover();
void mutation();
void printCumulativeFitness();
void printChromosome();
void printGeneration(int);
void calculateFitness(int numTestCase);
int main(){

    knapsackFile.open("D:\\Semester7\\softComputing\\ass\\ass1\\knapsack_input.txt");
    outFile.open("D:\\Semester7\\softComputing\\ass\\ass1\\Knapasck_output.txt");

    knapsackFile >> numTestCases >> sizKnapsack >> numItems;
    //Allocating the row space in heap dynamically
     items = new int * [numItems];
    //Allocating the value space in heap dynamically
    for (int i = 0; i < numItems; i++) {
        items[i] = new int[numItems];
    }

    //Allocating the row space in heap dynamically
    populations = new int * [numOfChromosome];
    //Allocating the value space in heap dynamically
    for (int i = 0; i < numItems; i++) {
        populations[i] = new int[numItems];
    }

    chromosome = new int[numItems];
    fitness = new int[numOfChromosome];
    //  loop number of test cases
    for (int i = 0; i < numTestCases; i++) {
        insertItems();
        setPopulation();
        setFitness();
        for (int j = 1; j <= maxGenerations ; ++j, change = 0) {
            printCumulativeFitness();
            selection();
            crossover();
            mutation();
            printGeneration(j);
            // to stop No New Chromosomes
            if(!change)
                  break;
        }calculateFitness(i);
    } outFile.close();
}

void insertItems(){
    for (int i = 0; i < numItems; ++i) {
        for (int j = 0; j < 2; ++j) {
            knapsackFile >> items[i][j];
        }
    }

}


int cumulativeFitness(int index){
    int sum = 0;
    for (int i = 0; i <= index; ++i) {
        sum += fitness[i];
    }
    return sum;
}

void setFitness(){
    cout << "\n<Step2> evaluate the fitness of each chromosome\n [ ";
    for (int i = 0; i < numOfChromosome; ++i) {
        fitness[i] = toDec(populations[i]);
        cout << fitness[i]<< " ";
    } cout << "] \n";
}

int calculateValue(int ch[]){
    int sum = 0;
    for (int i = 0; i < numItems; ++i)
        if(ch[i] == 1){
            sum += items[i][1];
        } return sum;
}

int calculateWeight(int ch[]){
    int sum = 0;
    for (int i = 0; i < numItems; ++i)
        if(ch[i] == 1){
            sum += items[i][0];
        } return sum;
}

bool isFitKnapsack(int ch[]){
    return calculateWeight(ch) <= sizKnapsack;
}

int toDec(int bin[]) { 
    int dec = 0, i = 0 , j = 0;
    for (i = numItems - 1; i >= 0 ; j++, i--) {
        if(bin[i]){
            dec += pow(2, j);
        }
    }return dec;
}


bool isFound(int ch[]){
    for (int i = 0; i < numOfChromosome; i++) {
        if(toDec(ch) == toDec(populations[i]))
            return true;
    }return false;
}

void copyChromosome(int ch[], int emptyChrom){
    for (int i = 0; i < numItems; i++) {
        populations[emptyChrom][i] = ch[i];
    }
}

void generateRandomChromosome(){
    srand(time(nullptr));
    for (int i = 0; i < numItems; i++) {
        chromosome[i] = rand() % (1 - 0 + 1) + 0;
    }
}

void setPopulation(){
    for (int i = 0; i < numOfChromosome; i++) {
        while (isFound(chromosome) || !isFitKnapsack(chromosome)){
            generateRandomChromosome();
        }copyChromosome(chromosome, i);
    }
    printChromosome();
}

int getRandom(){
    int lastFitness = numOfChromosome - 1;
    int cumLastFit = cumulativeFitness(lastFitness);
    return rand() % (cumLastFit - 0 + 1) + 0;
}

int selectChromosome(int r){
    if(r >= 0 && r <= cumulativeFitness(0))
        return 0;

    for (int i = 1, j = 0; i <= numOfChromosome - 1; i++, j++) {
        if(r >= cumulativeFitness(j) && r <= cumulativeFitness(i))
              return j;
    }
}

void selection(){
    int r1 , r2;
    r1 = getRandom();
    r2 = getRandom();
    firstSelChro = selectChromosome(r1);
    secondSelChro = selectChromosome(r2);
    while (firstSelChro == secondSelChro){r2 = getRandom(); secondSelChro = selectChromosome(r2);}
    cout << "\n<Step4> Selection \n r1 = " << r1 << " r2 = " << r2 << "\n firstSelChro = " << fitness[selectChromosome(r1)] << "\n secondSelChro = " << fitness[selectChromosome(r2)] <<endl;
}

void printNewChromosome(){
    for (int i = 0; i < numItems; ++i) {
        cout << populations[firstSelChro][i] << " ";
    } cout << "]   [ ";

    for (int i = 0; i < numItems; ++i) {
        cout << populations[secondSelChro][i] << " ";
    } cout << "] ";
    int newFirstSelChro = toDec(populations[firstSelChro]);
    int  newSecondSelChro = toDec(populations[secondSelChro]);
    cout << " => " <<newFirstSelChro << " : " << newSecondSelChro;

}

void swapCrossover(int point){
    for (int i = point; i < numItems; i++)
        swap(populations[firstSelChro][i], populations[secondSelChro][i]);
}

void crossover(){
    cout << "\n<Step5> Crossover between [ "; printNewChromosome();

    srand(time(nullptr));
    int r1 =  rand() % ((numItems - 1) - 1 + 1) + 1;
    float  r2 ,pc;
    cout << "\n r1 = " << r1;
    r2 =  0.0 + static_cast<float>(rand() / (RAND_MAX/((float)(1.0) - 0.0)));
    cout << "\n r2 = " << r2;
    pc =  0.5 + static_cast<float>(rand() / (RAND_MAX/((float)(0.7 - 0.4) - 0.4)));
    cout << "\n pc = " << pc;

    // check constrain of crossover, and FitKnapsack of new chromosome (is accepted? or return to parent)
    if (r2 <= pc){
        change = 1;
        swapCrossover(r1);
        if(!isFitKnapsack(populations[firstSelChro]) || !isFitKnapsack(populations[secondSelChro]))
            swapCrossover(r1);
        else
        cout << "\n newChromosome :  [ "; printNewChromosome();
    } else {
        cout << "\n no crossover";
    }
}

void flip(int index){
    srand(time(nullptr));
    float r, pm = 0.001 + static_cast<float>(rand() / (RAND_MAX/((float)(0.1 - 0.001) - 0.001)));

    for (int i = 0; i < numItems; ++i) {
        r =  0.0 + static_cast<float>(rand() / (RAND_MAX/((float)(1.0 - 0.0) - 0.0)));
        if(r <= pm){
            change = 1;
            if(populations[index][i])
                populations[index][i] = 0;
            else
                populations[index][i] = 1;
        }
    }
}


void mutation(){
    cout << "\n\n <step5> Mutation ";
    flip(firstSelChro);
    flip(secondSelChro);
    cout << "\n newChromosome :  [ "; printNewChromosome();

}

int getIndexOfFitness(){
    int indexOfFitness = 0; int maxWeight = 0;
    for (int i = 0, tempWeight; i < numOfChromosome; i++) {
        tempWeight = calculateWeight(populations[i]);
        if(tempWeight > maxWeight){
            indexOfFitness = i;
            maxWeight = tempWeight;
        }
    }return indexOfFitness;
}


int countElement(int chromo[]){
    int i , count;
    for ( i = 0, count = 0; i < numItems; ++i) {
        if(chromo[i])
            count++;
    }return count;
}

void calculateFitness(int numTestCase){

    outFile << "Test Case #" << numTestCase + 1;
    int indexOfFitness = getIndexOfFitness();

    outFile << "\nTotal Weight : " << calculateWeight(populations[indexOfFitness]);
    outFile << "\nTotal Value : " << calculateValue(populations[indexOfFitness]);
    outFile << "\nElements Count : " << countElement(populations[indexOfFitness])<< endl;
    for ( int i = 0; i < numItems; i++) {
        if(populations[indexOfFitness][i]){
            outFile << "(" << items[i][0] << " , " <<  items[i][1] << ")  ";}
    }outFile << endl << endl;
}


void printChromosome(){
   cout << "\n<Step1>\n randomly initialized chromosomes:\n";
    for (int i = 0; i < numOfChromosome; ++i) {
        cout << " C" << i+1 << " = [ ";
        for (int j = 0; j < numItems; ++j) {
            cout << populations[i][j] << " ";
        } cout << "]\n";
    }
}

void printCumulativeFitness(){
    cout << "\n<Step3> calculate the cumulative fitness\n [ ";
    for (int i = 0; i < numOfChromosome; ++i) {
       cout <<  cumulativeFitness(i) << " ";
    } cout << "] \n";
}

void printGeneration(int numGenerat){
       cout << " \n\nGeneration #" << numGenerat << " : \n";
       cout << "  Chromosome      Binary         Fitness        Weight         value\n";
    for (int i = 0, j; i < numOfChromosome; i++) {
        cout << "     C" << i << "            ";
        for (j = 0; j < numItems; j++) {
            cout << populations[i][j] ;
        } cout << "            " << toDec(populations[i]) ;
          cout << "            " << calculateWeight(populations[i]);
          cout << "            " << calculateValue(populations[i]) << "\n";
    }
}