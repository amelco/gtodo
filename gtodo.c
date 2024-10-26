#include <stdlib.h>
#include <string.h>

#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <sqlite3.h>
#define SQL_API_IMPLEMENTATION
#include "sqlapi.h"

// # .........................
// # -- Janela de alunos
// #
struct janela_lista_alunos {
    bool mostra;
    int scrollIndex;
    int active;
    char nome[256];
    bool nome_edit;
    char* alunos;
};

void init_janela_lista_alunos(struct janela_lista_alunos* jListaAlunos) {
    char* alunos = "Fulano de Tal;Beltrano da Silva";
    jListaAlunos->alunos = malloc(strlen(alunos));
    strcpy(jListaAlunos->alunos, alunos);

    jListaAlunos->mostra = false;
    jListaAlunos->scrollIndex = 0;
    jListaAlunos->active = 0;
    jListaAlunos->nome_edit = false;
}

void janela_lista_alunos(struct janela_lista_alunos* jListaAlunos)
{
    int result = GuiWindowBox((Rectangle){ 150, 100, 350, 250 }, "#177#Lista de alunos");
    if (result != 0) jListaAlunos->mostra = false;
    else{
        GuiListView((Rectangle){ 160, 130, 130, 150 }, jListaAlunos->alunos, &jListaAlunos->scrollIndex, &jListaAlunos->active);
        GuiButton((Rectangle){ 160, 300, 80, 20 }, "#113#Deletar");

        GuiLabel((Rectangle){ 310, 130, 80, 20 }, "Nome");
        if (GuiTextBox((Rectangle){ 350, 130, 80, 20 }, jListaAlunos->nome, 15, jListaAlunos->nome_edit)) jListaAlunos->nome_edit = !jListaAlunos->nome_edit;
        if (GuiButton((Rectangle){ 350, 160, 80, 20 }, "Adicionar")) {
            int size = strlen(jListaAlunos->alunos) + strlen(jListaAlunos->nome);
            char* novo = malloc(size);
            memset(novo, 0, size);
            strcpy(novo, jListaAlunos->alunos);
            strcat(novo, ";");
            strcat(novo, jListaAlunos->nome);
            free(jListaAlunos->alunos);
            jListaAlunos->alunos = novo;
        }
    }
}
// #
// # .........................

int main()
{

    InitWindow(800, 600, "CRUD");
    SetTargetFPS(30);

    sqlite3 *db;
    char* conn_string = "db_ceman.db";
    if (sql_open(conn_string, &db) == ERROR) {
        TraceLog(LOG_INFO, "Error: %s", sql_get_errormsg());
        exit(3440);
    }

    char response[1024];
    if (sql_read(db, "select * from instituicao", &response[0]) == ERROR) {
        TraceLog(LOG_INFO, "Error: %s", sql_get_errormsg());
        exit(3440);
    }

    if (sql_close(db) == ERROR) {
        TraceLog(LOG_INFO, "Error: %s", sql_get_errormsg());
        exit(3440);
    }
    return 0;

    TraceLog(LOG_INFO, "Connected to db %s", conn_string);

    struct janela_lista_alunos jListaAlunos = {0};
    init_janela_lista_alunos(&jListaAlunos);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (GuiButton((Rectangle){ 24, 24, 120, 30 }, "#177#Listar alunos")) jListaAlunos.mostra = true;

        if (jListaAlunos.mostra)
        {
            janela_lista_alunos(&jListaAlunos);
        }

        EndDrawing();
    }

    free(jListaAlunos.alunos);
    CloseWindow();
    return 0;
}
