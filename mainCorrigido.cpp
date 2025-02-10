#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <random>

using namespace std;

// g++ mainCorrigido.cpp -pthread -o corrigido
// Versão sem problemas

const int POLTRONAS = 10;
const int PASSAGEIROS = 15;
const int MAX_VIAGENS = 3;

const int MIN_DESCANSO = 0;
const int MAX_DESCANSO = 6;

vector<int> assentos(POLTRONAS, -1);
vector<bool> pessoa_embarcada(PASSAGEIROS, false);
vector<bool> precisa_descansar(PASSAGEIROS, false);
vector<chrono::steady_clock::time_point> tempo_descanso(PASSAGEIROS);

int total_ocupadas = 0;
int viagens = 0;

// Sincronização
mutex mtx;
condition_variable cv_onibus;
condition_variable cv_pessoas;
bool em_viagem = false;

void pessoa(int id) {
    while (viagens < MAX_VIAGENS) {
        // Verifica se precisa descansar
        if (precisa_descansar[id]) {
            if (chrono::steady_clock::now() < tempo_descanso[id]) {
                this_thread::sleep_for(chrono::milliseconds(100));
                continue;
            }
            precisa_descansar[id] = false;
        }

        unique_lock<mutex> lock(mtx);
        
        while (em_viagem) {
            cv_pessoas.wait(lock);
        }
        
        if (total_ocupadas < POLTRONAS && viagens < MAX_VIAGENS) {
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> dis(0, POLTRONAS - 1);
            int poltrona = dis(gen);
            
            if (assentos[poltrona] == -1 && !pessoa_embarcada[id]) {
                assentos[poltrona] = id;
                pessoa_embarcada[id] = true;
                total_ocupadas++;
                cout << "Pessoa " << id << " sentou na poltrona " << poltrona << endl;
                
                if (total_ocupadas == POLTRONAS) {
                    cv_onibus.notify_one();
                }

                // Tempo de descanso
                uniform_int_distribution<> dis_tempo(MIN_DESCANSO, MAX_DESCANSO);
                tempo_descanso[id] = chrono::steady_clock::now() + chrono::seconds(dis_tempo(gen));
                precisa_descansar[id] = true;
            }
        }
        
        lock.unlock();
        this_thread::sleep_for(chrono::milliseconds(50));
    }
}

void onibus() {
    while (viagens < MAX_VIAGENS) {
        unique_lock<mutex> lock(mtx);
        
        // Espera até que todas as poltronas estejam ocupadas
        while (total_ocupadas < POLTRONAS && viagens < MAX_VIAGENS) {
            cv_onibus.wait(lock);
        }
        
        if (viagens < MAX_VIAGENS) {
            em_viagem = true;
            
            cout << "\nEstado dos assentos:\n";
            for (int i = 0; i < POLTRONAS; i++) {
                cout << "Poltrona " << i << ": Pessoa " << assentos[i] << endl;
            }
            
            cout << "\nOnibus partiu! Total ocupadas: " << total_ocupadas << endl;
            lock.unlock();
            
            this_thread::sleep_for(chrono::seconds(2));
            
            lock.lock();
            for (int i = 0; i < POLTRONAS; i++) {
                assentos[i] = -1;
            }
            fill(pessoa_embarcada.begin(), pessoa_embarcada.end(), false);  // Reseta o controle de pessoas embarcadas
            total_ocupadas = 0;
            viagens++;
            em_viagem = false;
            
            cout << "Onibus voltou! Viagem " << viagens << " concluida\n" << endl;
            
            // Notifica todas as pessoas que o ônibus voltou
            cv_pessoas.notify_all();
        }
        
        lock.unlock();
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