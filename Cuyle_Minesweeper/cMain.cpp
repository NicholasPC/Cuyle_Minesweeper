#include "cMain.h"

wxBEGIN_EVENT_TABLE(cMain, wxFrame)
	EVT_BUTTON(10001, OnButtonClicked)
wxEND_EVENT_TABLE()

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Minesweeper!", wxPoint(30, 30), wxSize(800, 600))
{
	btn = new wxButton * [fieldWidth * fieldHeight];
	wxGridSizer* grid = new wxGridSizer(fieldWidth, fieldHeight, 0, 0);

	field = new int[fieldWidth * fieldHeight];

	wxFont font(24, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);

	for (int i = 0; i < fieldWidth; i++) {
		for (int j = 0; j < fieldHeight; j++) {
			btn[j * fieldWidth + i] = new wxButton(this, 10000 + (j * fieldWidth + i));
			btn[j * fieldWidth + i]->SetFont(font);
			grid->Add(btn[j * fieldWidth + i], 1, wxEXPAND | wxALL);

			btn[j * fieldWidth + i]->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cMain::OnButtonClicked, this);
			field[j * fieldWidth + i] = 0;
		}
	}

	this->SetSizer(grid);
	grid->Layout();
}

cMain::~cMain() {
	delete[]btn;
}

void cMain::OnButtonClicked(wxCommandEvent& evt) {
	int x = (evt.GetId() - 10000) % fieldWidth;
	int y = (evt.GetId() - 10000) / fieldWidth;

	if (firstClick) {
		int mines = 30;

		while (mines) {
			int randX = rand() % fieldWidth;
			int randY = rand() % fieldHeight;

			if (field[randY * fieldWidth + randX] == 0 && randX != x && randY != y) {
				field[randY * fieldWidth + randX] = -1;
				mines--;
			}
		}

		firstClick = false;
	}
	btn[y * fieldWidth + x]->Enable(false);

	if (field[y * fieldWidth + x] == -1) {
		wxMessageBox("KA-BLAMO!!! You Blew Up :(");
		
		firstClick = true;
		for (int i = 0; i < fieldWidth; i++) {
			for (int j = 0; j < fieldHeight; j++) {
				field[j * fieldWidth + i] = 0;
				btn[j * fieldWidth + i]->SetLabel("");
				btn[y * fieldWidth + i]->Enable(true);
			}
		}
	}
	else {
		int mineCount = 0;
		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				if (x + i >= 0 && x + i < fieldWidth && y + j >= 0 && y + j < fieldHeight) {
					if (field[(y + j) * fieldWidth + (x + i)] == -1) {
						mineCount++;
					}
				}
			}
		}

		if (mineCount > 0) {
			btn[y * fieldWidth + x]->SetLabel(std::to_string(mineCount));
		}
	}

	evt.Skip();
}
