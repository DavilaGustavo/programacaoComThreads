#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <random>

using namespace std;

// g++ mainProblema.cpp -pthread -o problema
// Versão com problemas

const int POLTRONAS = 10;
const int PASSAGEIROS = 15;
const int MAX_VIAGENS = 3;

const int MIN_DESCANSO = 0;
const int MAX_DESCANSO = 6;

vector<int> assentos(POLTRONAS, -1);
int total_ocupadas = 0;
int viagens = 0;

void pessoa(int id) {
    while (viagens < MAX_VIAGENS) {
        if (total_ocupadas < POLTRONAS) {
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> dis(0, POLTRONAS - 1);
            int poltrona = dis(gen);
            
            if (assentos[poltrona] == -1) {
                this_thread::sleep_for(chrono::milliseconds(1));
                
                assentos[poltrona] = id;
                total_ocupadas++;
                cout << "Pessoa " << id << " sentou na poltrona " << poltrona << endl;
                this_thread::sleep_for(chrono::milliseconds(200));
            }
        }
        this_thread::sleep_for(chrono::milliseconds(50));
    }
}

void onibus() {
    while (viagens < MAX_VIAGENS) {
        if (total_ocupadas >= POLTRONAS) { 
            cout << "\nEstado dos assentos:\n";
            for (int i = 0; i < POLTRONAS; i++) {
                cout << "Poltrona " << i << ": Pessoa " << assentos[i] << endl;
            }
            
            cout << "\nOnibus partiu! Total ocupadas: " << total_ocupadas << endl;
            this_thread::sleep_for(chrono::seconds(2));
            
            for (int i = 0; i < POLTRONAS; i++) {
                assentos[i] = -1;
            }
            total_ocupadas = 0;
            viagens++;
            
            cout << "Onibus voltou! Viagem " << viagens << " concluida\n" << endl;
            
            this_thread::sleep_for(chrono::milliseconds(500));
        }
        this_thread::sleep_for(chrono::milliseconds(50));
    }
}

int main() {
    vector<thread> threads;
    
    thread t_onibus(onibus);
    
    for (int i = 0; i < PASSAGEIROS; i++) {
        threads.push_back(thread(pessoa, i));
        this_thread::sleep_for(chrono::milliseconds(10));
    }
    
    for (auto& t : threads) {
        t.join();
    }
    t_onibus.join();
    
    return 0;
}