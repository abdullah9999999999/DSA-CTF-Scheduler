#pragma once
#include <fstream>
using namespace std;

namespace DSP {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    public ref class schedule : public System::Windows::Forms::Form
    {

    public:
        Point mouseLocation;
        schedule(void)
        {
            InitializeComponent();
            randomGenerator = gcnew Random(); // Initialize random generator
            InitializeSchedule(); // Initialize the schedule properly
        }

    protected:
        ~schedule()
        {
            if (components)
            {
                delete components;
            }
        }

    private:
        System::Windows::Forms::DataGridView^ dataGridViewRound1;
        System::Windows::Forms::DataGridView^ dataGridViewRound2;
        System::Windows::Forms::DataGridView^ dataGridViewRound3;
        System::Windows::Forms::Button^ ex_button;
        System::Windows::Forms::Button^ updateButton;
        System::Windows::Forms::TextBox^ winnerTextBox;
        System::Windows::Forms::TextBox^ finalMatchTextBox;
        System::Windows::Forms::Label^ label1;
        System::Windows::Forms::Label^ label2;
        System::Windows::Forms::Label^ label3;
        System::ComponentModel::Container^ components;
        System::Windows::Forms::Label^ label4;
        System::Windows::Forms::Label^ label5;
        System::Windows::Forms::Label^ label6;
        Random^ randomGenerator;

        string queueTeams;
        int queueFront;
        int queueBack;
        string stackWinners;
        int stackTop;
        bool winnerAnnounced = false;

#pragma region Windows Form Designer generated code

        void InitializeSchedule()
        {
            //16 teams in total and 16 ranks for each team
            const int numTeams = 16;
            string teams = gcnew string(numTeams);
            int teamRanks = gcnew int(numTeams);

            //Initialize queue
            queueTeams = gcnew string(numTeams);
            queueFront = 0;
            queueBack = 0;

            //Initialize stack
            stackWinners = gcnew string(numTeams);
            stackTop = -1;

            //Read teams from file
            ReadTeamsFromFile(teams, teamRanks, numTeams);

            //Divide teams into two groups
            string groupA = gcnew string(8);
            string groupB = gcnew string(8);

            //Function to divide teams into two groups
            DivideTeams(teams, teamRanks, groupA, groupB, numTeams);

            //Schedule matches for the first round
            ScheduleMatches(groupA, groupB, 8, dataGridViewRound1);
        }

        void ReadTeamsFromFile(string teams, int teamRanks, int numTeams)
        {
            ifstream file("E:\\CTFdata.txt");
            if (!file.is_open()) {
                MessageBox::Show("Error opening file!", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
                return;
            }

            char line[64];
            int i = 0;

            while (i < numTeams && file.getline(line, sizeof(line)))
            {
                char* teamName = line;
                char* rankStr = nullptr;

                for (int j = 0; line[j] != '\0'; ++j) {
                    if (line[j] == ':') {
                        line[j] = '\0';
                    }
                    else if (line[j] == ',' && rankStr == nullptr) {
                        rankStr = &line[j + 1];
                        line[j] = '\0';
                    }
                }

                if (teamName && rankStr) { //if both teamName and rankStr are not null
                    teams[i] = gcnew string(teamName); //convert char* to string
                    int rank = 0;
                    for (int j = 0; rankStr[j] != '\0'; ++j) {
                        if (rankStr[j] >= '0' && rankStr[j] <= '9') {
                            rank = rank * 10 + (rankStr[j] - '0');
                        }
                    }
                    teamRanks[i] = rank;
                }
                ++i;
            }

            file.close();
        }

        void DivideTeams(string teams, int teamRanks, string groupA, string groupB, int numTeams)
        {
            // Sort the teams by rank
            for (int i = 0; i < numTeams - 1; ++i) {
                for (int j = i + 1; j < numTeams; ++j) {
                    if (teamRanks[i] > teamRanks[j]) {
                        int tempRank = teamRanks[i];
                        teamRanks[i] = teamRanks[j];
                        teamRanks[j] = tempRank;

                        String^ tempTeam = teams[i];
                        teams[i] = teams[j];
                        teams[j] = tempTeam;
                    }
                }
            }

            for (int i = 0; i < numTeams; ++i) {
                if (i < 8) {
                    groupA[i] = teams[i];
                }
                else {
                    groupB[i - 8] = teams[i];
                }
            }
        }

        void ScheduleMatches(string groupA, string groupB, int groupSize, System::Windows::Forms::DataGridView^ target)
        {
            if (target->Columns->Count == 0) {
                target->Columns->Add("MatchNumber", "Match Number");
                target->Columns->Add("TeamA", "Team A");
                target->Columns->Add("TeamB", "Team B");
                target->Columns->Add("Date", "Date");
                target->Columns->Add("Time", "Time");
                target->Columns->Add("Winner", "Winner");
            }

            DateTime date = DateTime::Now;

            for (int i = 0; i < groupSize; ++i) {
                string teamA = groupA[i];
                string teamB = groupB[i];

                if (!string::IsNullOrEmpty(teamA) && !string::IsNullOrEmpty(teamB)) {
                    String^ winner = (randomGenerator->Next(2) == 0) ? teamA : teamB;

                    target->Rows->Add(
                        (i + 1).ToString(),
                        teamA,
                        teamB,
                        date.AddDays(i).ToString("yyyy-MM-dd"),
                        date.AddHours(14 + (i % 3) * 2).ToString("hh:mm tt"),
                        winner
                    );

                    Enqueue(winner);
                }
            }
        }

        void Enqueue(string team)
        {
            if (queueBack >= queueTeams->Length) {
                return;
            }

            queueTeams[queueBack++] = team;
        }

        string Dequeue()
        {
            if (queueFront < queueBack) {
                return queueTeams[queueFront++];
            }
            return nullptr;
        }

        void GenerateNextRoundMatches(System::Windows::Forms::DataGridView^ currentRound,
            System::Windows::Forms::DataGridView^ nextRound,
            int numMatches)
        {
            nextRound->Columns->Clear();
            nextRound->Rows->Clear();

            nextRound->Columns->Add("MatchNumber", "Match Number");
            nextRound->Columns->Add("TeamA", "Team A");
            nextRound->Columns->Add("TeamB", "Team B");
            nextRound->Columns->Add("Date", "Date");
            nextRound->Columns->Add("Time", "Time");
            nextRound->Columns->Add("Winner", "Winner");

            DateTime date = DateTime::Now.AddDays(7);
            int nextMatchNumber = 1;

            for (int i = 0; i < numMatches; i++) {
                if (i * 2 + 1 < currentRound->Rows->Count) {
                    string teamA = currentRound->Rows[i * 2]->Cells["Winner"]->Value->ToString();
                    string teamB = currentRound->Rows[i * 2 + 1]->Cells["Winner"]->Value->ToString();

                    if (!string::IsNullOrEmpty(teamA) && !string::IsNullOrEmpty(teamB)) {
                        string winner = (randomGenerator->Next(2) == 0) ? teamA : teamB;

                        nextRound->Rows->Add(
                            (nextMatchNumber++).ToString(),
                            teamA,
                            teamB,
                            date.AddDays(i).ToString("yyyy-MM-dd"),
                            date.AddHours(14 + (i % 3) * 2).ToString("hh:mm tt"),
                            winner
                        );

                        if (stackTop < stackWinners->Length - 1) {
                            stackWinners[++stackTop] = winner;
                        }
                    }
                }
            }
        }

        void OnUpdateButtonClick(System::Object^ sender, System::EventArgs^ e)
        {
            if (winnerAnnounced)
            {
                MessageBox::Show(
                    "Tournament Winner has already been announced!\n" +
                    winnerTextBox->Text,
                    "Tournament Completed",
                    MessageBoxButtons::OK,
                    MessageBoxIcon::Information
                );
                return;
            }

            if (dataGridViewRound1 == nullptr || dataGridViewRound2 == nullptr || dataGridViewRound3 == nullptr)
            {
                MessageBox::Show("One or more DataGridViews are not initialized.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
                return;
            }

            if (dataGridViewRound1->Rows->Count > 0 && dataGridViewRound2->Rows->Count == 0)
            {
                GenerateNextRoundMatches(dataGridViewRound1, dataGridViewRound2, 4);
            }
            else if (dataGridViewRound2->Rows->Count > 0 && dataGridViewRound3->Rows->Count == 0)
            {
                GenerateNextRoundMatches(dataGridViewRound2, dataGridViewRound3, 2);
            }
            else if (dataGridViewRound3->Rows->Count > 0)
            {
                if (stackTop >= 1)
                {
                    if (stackWinners == nullptr || stackWinners->Length <= stackTop)
                    {
                        MessageBox::Show(
                            "Winner stack is not properly initialized or is invalid.",
                            "Error",
                            MessageBoxButtons::OK,
                            MessageBoxIcon::Error
                        );
                        return;
                    }

                    string finalTeamA = stackWinners[stackTop];
                    stackTop--;
                    string finalTeamB = stackWinners[stackTop];
                    stackTop--;

                    finalMatchTextBox->Text = finalTeamA + " VS " + finalTeamB;

                    if (randomGenerator == nullptr)
                    {
                        MessageBox::Show(
                            "Random generator is not initialized.",
                            "Error",
                            MessageBoxButtons::OK,
                            MessageBoxIcon::Error
                        );
                        return;
                    }

                    string finalWinner = (randomGenerator->Next(2) == 0) ? finalTeamA : finalTeamB;
                    winnerTextBox->Text = "Winner: " + finalWinner;

                    winnerAnnounced = true;
                    updateButton->Enabled = false;
                }
                else
                {
                    MessageBox::Show(
                        "Not enough teams for final match. Please reset the tournament.",
                        "Error",
                        MessageBoxButtons::OK,
                        MessageBoxIcon::Error
                    );
                }
            }
            else
            {
                MessageBox::Show("All rounds are completed.", "Info", MessageBoxButtons::OK, MessageBoxIcon::Information);
            }

        }
    private: System::Void mouse_Down(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
        mouseLocation = Point(-e->X, -e->Y);
    }
    private: System::Void mouse_Move(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
        if (e->Button == Windows::Forms::MouseButtons::Left) {
            Point mousePose = Control::MousePosition;
            mousePose.Offset(mouseLocation.X, mouseLocation.Y);
            Location = mousePose;
        }
    }


    };
#pragma region Windows Form Designer generated code
        void InitializeComponent(void)
        {
            System::Windows::Forms::DataGridViewCellStyle^ dataGridViewCellStyle1 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^ dataGridViewCellStyle2 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^ dataGridViewCellStyle3 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^ dataGridViewCellStyle4 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^ dataGridViewCellStyle5 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^ dataGridViewCellStyle6 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^ dataGridViewCellStyle7 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^ dataGridViewCellStyle8 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            this->dataGridViewRound1 = (gcnew System::Windows::Forms::DataGridView());
            this->dataGridViewRound2 = (gcnew System::Windows::Forms::DataGridView());
            this->dataGridViewRound3 = (gcnew System::Windows::Forms::DataGridView());
            this->updateButton = (gcnew System::Windows::Forms::Button());
            this->winnerTextBox = (gcnew System::Windows::Forms::TextBox());
            this->finalMatchTextBox = (gcnew System::Windows::Forms::TextBox());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->label2 = (gcnew System::Windows::Forms::Label());
            this->label3 = (gcnew System::Windows::Forms::Label());
            this->label4 = (gcnew System::Windows::Forms::Label());
            this->label5 = (gcnew System::Windows::Forms::Label());
            this->label6 = (gcnew System::Windows::Forms::Label());
            this->ex_button = (gcnew System::Windows::Forms::Button());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridViewRound1))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridViewRound2))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridViewRound3))->BeginInit();
            this->SuspendLayout();
            // 
            // dataGridViewRound1
            // 
            this->dataGridViewRound1->AccessibleRole = System::Windows::Forms::AccessibleRole::None;
            this->dataGridViewRound1->AllowUserToAddRows = false;
            this->dataGridViewRound1->AllowUserToDeleteRows = false;
            this->dataGridViewRound1->AllowUserToResizeRows = false;
            dataGridViewCellStyle1->ForeColor = System::Drawing::Color::Black;
            dataGridViewCellStyle1->SelectionBackColor = System::Drawing::Color::Black;
            dataGridViewCellStyle1->SelectionForeColor = System::Drawing::Color::White;
            this->dataGridViewRound1->AlternatingRowsDefaultCellStyle = dataGridViewCellStyle1;
            this->dataGridViewRound1->BackgroundColor = System::Drawing::Color::Black;
            this->dataGridViewRound1->ColumnHeadersBorderStyle = System::Windows::Forms::DataGridViewHeaderBorderStyle::Single;
            dataGridViewCellStyle2->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
            dataGridViewCellStyle2->BackColor = System::Drawing::Color::Black;
            dataGridViewCellStyle2->Font = (gcnew System::Drawing::Font(L"Agency FB", 10, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            dataGridViewCellStyle2->ForeColor = System::Drawing::Color::White;
            dataGridViewCellStyle2->SelectionBackColor = System::Drawing::Color::Black;
            dataGridViewCellStyle2->SelectionForeColor = System::Drawing::Color::White;
            dataGridViewCellStyle2->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
            this->dataGridViewRound1->ColumnHeadersDefaultCellStyle = dataGridViewCellStyle2;
            this->dataGridViewRound1->ColumnHeadersHeight = 34;
            dataGridViewCellStyle3->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            dataGridViewCellStyle3->BackColor = System::Drawing::Color::Black;
            dataGridViewCellStyle3->Font = (gcnew System::Drawing::Font(L"Agency FB", 10, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            dataGridViewCellStyle3->ForeColor = System::Drawing::Color::White;
            dataGridViewCellStyle3->SelectionBackColor = System::Drawing::Color::Black;
            dataGridViewCellStyle3->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle3->WrapMode = System::Windows::Forms::DataGridViewTriState::False;
            this->dataGridViewRound1->DefaultCellStyle = dataGridViewCellStyle3;
            this->dataGridViewRound1->GridColor = System::Drawing::Color::Black;
            this->dataGridViewRound1->Location = System::Drawing::Point(12, 12);
            this->dataGridViewRound1->Name = L"dataGridViewRound1";
            this->dataGridViewRound1->ReadOnly = true;
            this->dataGridViewRound1->RowHeadersBorderStyle = System::Windows::Forms::DataGridViewHeaderBorderStyle::Single;
            dataGridViewCellStyle4->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
            dataGridViewCellStyle4->BackColor = System::Drawing::SystemColors::Control;
            dataGridViewCellStyle4->Font = (gcnew System::Drawing::Font(L"Agency FB", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            dataGridViewCellStyle4->ForeColor = System::Drawing::Color::White;
            dataGridViewCellStyle4->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle4->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle4->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
            this->dataGridViewRound1->RowHeadersDefaultCellStyle = dataGridViewCellStyle4;
            this->dataGridViewRound1->RowHeadersVisible = false;
            this->dataGridViewRound1->RowHeadersWidth = 62;
            this->dataGridViewRound1->RowHeadersWidthSizeMode = System::Windows::Forms::DataGridViewRowHeadersWidthSizeMode::DisableResizing;
            this->dataGridViewRound1->RowTemplate->DefaultCellStyle->ForeColor = System::Drawing::Color::White;
            this->dataGridViewRound1->RowTemplate->DefaultCellStyle->SelectionBackColor = System::Drawing::Color::Black;
            this->dataGridViewRound1->RowTemplate->ReadOnly = true;
            this->dataGridViewRound1->Size = System::Drawing::Size(713, 268);
            this->dataGridViewRound1->TabIndex = 0;
            // 
            // dataGridViewRound2
            // 
            this->dataGridViewRound2->AllowUserToAddRows = false;
            this->dataGridViewRound2->AllowUserToDeleteRows = false;
            this->dataGridViewRound2->AllowUserToResizeRows = false;
            this->dataGridViewRound2->BackgroundColor = System::Drawing::Color::Black;
            this->dataGridViewRound2->ColumnHeadersBorderStyle = System::Windows::Forms::DataGridViewHeaderBorderStyle::Single;
            dataGridViewCellStyle5->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
            dataGridViewCellStyle5->BackColor = System::Drawing::Color::Black;
            dataGridViewCellStyle5->Font = (gcnew System::Drawing::Font(L"Agency FB", 10, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            dataGridViewCellStyle5->ForeColor = System::Drawing::Color::White;
            dataGridViewCellStyle5->SelectionBackColor = System::Drawing::Color::Black;
            dataGridViewCellStyle5->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle5->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
            this->dataGridViewRound2->ColumnHeadersDefaultCellStyle = dataGridViewCellStyle5;
            this->dataGridViewRound2->ColumnHeadersHeight = 34;
            dataGridViewCellStyle6->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            dataGridViewCellStyle6->BackColor = System::Drawing::Color::Black;
            dataGridViewCellStyle6->Font = (gcnew System::Drawing::Font(L"Agency FB", 10, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            dataGridViewCellStyle6->ForeColor = System::Drawing::Color::White;
            dataGridViewCellStyle6->SelectionBackColor = System::Drawing::Color::Black;
            dataGridViewCellStyle6->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle6->WrapMode = System::Windows::Forms::DataGridViewTriState::False;
            this->dataGridViewRound2->DefaultCellStyle = dataGridViewCellStyle6;
            this->dataGridViewRound2->GridColor = System::Drawing::Color::Black;
            this->dataGridViewRound2->Location = System::Drawing::Point(14, 286);
            this->dataGridViewRound2->Name = L"dataGridViewRound2";
            this->dataGridViewRound2->ReadOnly = true;
            this->dataGridViewRound2->RowHeadersBorderStyle = System::Windows::Forms::DataGridViewHeaderBorderStyle::Single;
            this->dataGridViewRound2->RowHeadersVisible = false;
            this->dataGridViewRound2->RowHeadersWidth = 62;
            this->dataGridViewRound2->RowHeadersWidthSizeMode = System::Windows::Forms::DataGridViewRowHeadersWidthSizeMode::DisableResizing;
            this->dataGridViewRound2->Size = System::Drawing::Size(711, 150);
            this->dataGridViewRound2->TabIndex = 1;
            // 
            // dataGridViewRound3
            // 
            this->dataGridViewRound3->AllowUserToAddRows = false;
            this->dataGridViewRound3->AllowUserToDeleteRows = false;
            this->dataGridViewRound3->AllowUserToResizeRows = false;
            this->dataGridViewRound3->BackgroundColor = System::Drawing::Color::Black;
            dataGridViewCellStyle7->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            dataGridViewCellStyle7->BackColor = System::Drawing::Color::Black;
            dataGridViewCellStyle7->Font = (gcnew System::Drawing::Font(L"Agency FB", 10, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            dataGridViewCellStyle7->ForeColor = System::Drawing::Color::White;
            dataGridViewCellStyle7->SelectionBackColor = System::Drawing::Color::Black;
            dataGridViewCellStyle7->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle7->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
            this->dataGridViewRound3->ColumnHeadersDefaultCellStyle = dataGridViewCellStyle7;
            this->dataGridViewRound3->ColumnHeadersHeight = 34;
            dataGridViewCellStyle8->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            dataGridViewCellStyle8->BackColor = System::Drawing::Color::Black;
            dataGridViewCellStyle8->Font = (gcnew System::Drawing::Font(L"Agency FB", 10, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            dataGridViewCellStyle8->ForeColor = System::Drawing::Color::White;
            dataGridViewCellStyle8->SelectionBackColor = System::Drawing::Color::Black;
            dataGridViewCellStyle8->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle8->WrapMode = System::Windows::Forms::DataGridViewTriState::False;
            this->dataGridViewRound3->DefaultCellStyle = dataGridViewCellStyle8;
            this->dataGridViewRound3->GridColor = System::Drawing::Color::Black;
            this->dataGridViewRound3->Location = System::Drawing::Point(14, 442);
            this->dataGridViewRound3->Name = L"dataGridViewRound3";
            this->dataGridViewRound3->RowHeadersVisible = false;
            this->dataGridViewRound3->RowHeadersWidth = 62;
            this->dataGridViewRound3->RowHeadersWidthSizeMode = System::Windows::Forms::DataGridViewRowHeadersWidthSizeMode::DisableResizing;
            this->dataGridViewRound3->Size = System::Drawing::Size(711, 113);
            this->dataGridViewRound3->TabIndex = 2;
            // 
            // updateButton
            // 
            this->updateButton->FlatAppearance->BorderColor = System::Drawing::Color::White;
            this->updateButton->FlatAppearance->BorderSize = 2;
            this->updateButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->updateButton->Font = (gcnew System::Drawing::Font(L"Impact", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->updateButton->ForeColor = System::Drawing::Color::White;
            this->updateButton->Location = System::Drawing::Point(14, 561);
            this->updateButton->Name = L"updateButton";
            this->updateButton->Size = System::Drawing::Size(169, 40);
            this->updateButton->TabIndex = 3;
            this->updateButton->Text = L"Update";
            this->updateButton->Click += gcnew System::EventHandler(this, &schedule::OnUpdateButtonClick);
            // 
            // winnerTextBox
            // 
            this->winnerTextBox->BackColor = System::Drawing::Color::Black;
            this->winnerTextBox->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            this->winnerTextBox->Cursor = System::Windows::Forms::Cursors::No;
            this->winnerTextBox->Font = (gcnew System::Drawing::Font(L"Agency FB", 14, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->winnerTextBox->ForeColor = System::Drawing::Color::White;
            this->winnerTextBox->Location = System::Drawing::Point(315, 606);
            this->winnerTextBox->Name = L"winnerTextBox";
            this->winnerTextBox->ReadOnly = true;
            this->winnerTextBox->Size = System::Drawing::Size(300, 41);
            this->winnerTextBox->TabIndex = 4;
            this->winnerTextBox->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // finalMatchTextBox
            // 
            this->finalMatchTextBox->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            this->finalMatchTextBox->Cursor = System::Windows::Forms::Cursors::No;
            this->finalMatchTextBox->Font = (gcnew System::Drawing::Font(L"Agency FB", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->finalMatchTextBox->Location = System::Drawing::Point(201, 565);
            this->finalMatchTextBox->Name = L"finalMatchTextBox";
            this->finalMatchTextBox->ReadOnly = true;
            this->finalMatchTextBox->Size = System::Drawing::Size(524, 36);
            this->finalMatchTextBox->TabIndex = 5;
            this->finalMatchTextBox->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // label1
            // 
            this->label1->Location = System::Drawing::Point(0, 0);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(100, 23);
            this->label1->TabIndex = 0;
            this->label1->Text = L"QUARTER FINAL";
            // 
            // label2
            // 
            this->label2->Location = System::Drawing::Point(0, 0);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(100, 23);
            this->label2->TabIndex = 0;
            this->label2->Text = L"SEMI FINAL";
            // 
            // label3
            // 
            this->label3->Location = System::Drawing::Point(0, 0);
            this->label3->Name = L"label3";
            this->label3->Size = System::Drawing::Size(100, 23);
            this->label3->TabIndex = 0;
            this->label3->Text = L"FINAL";
            // 
            // label4
            // 
            this->label4->AutoSize = true;
            this->label4->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->label4->Font = (gcnew System::Drawing::Font(L"Wide Latin", 11, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->label4->ForeColor = System::Drawing::Color::White;
            this->label4->Location = System::Drawing::Point(795, 333);
            this->label4->Name = L"label4";
            this->label4->Size = System::Drawing::Size(234, 54);
            this->label4->TabIndex = 6;
            this->label4->Text = L"QUARTER \r\n       FINAL";
            // 
            // label5
            // 
            this->label5->AutoSize = true;
            this->label5->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->label5->Font = (gcnew System::Drawing::Font(L"Wide Latin", 11, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->label5->ForeColor = System::Drawing::Color::White;
            this->label5->Location = System::Drawing::Point(838, 460);
            this->label5->Name = L"label5";
            this->label5->Size = System::Drawing::Size(150, 54);
            this->label5->TabIndex = 7;
            this->label5->Text = L" SEMI \r\nFINAL";
            // 
            // label6
            // 
            this->label6->AutoSize = true;
            this->label6->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->label6->Font = (gcnew System::Drawing::Font(L"Wide Latin", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->label6->ForeColor = System::Drawing::Color::White;
            this->label6->Location = System::Drawing::Point(763, 110);
            this->label6->Name = L"label6";
            this->label6->Size = System::Drawing::Size(287, 48);
            this->label6->TabIndex = 8;
            this->label6->Text = L"ELIMINATION\r\n            ROUND\r\n";
            // 
            // ex_button
            // 
            this->ex_button->BackColor = System::Drawing::Color::White;
            this->ex_button->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->ex_button->Font = (gcnew System::Drawing::Font(L"Impact", 13, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->ex_button->ForeColor = System::Drawing::Color::Black;
            this->ex_button->Location = System::Drawing::Point(14, 607);
            this->ex_button->Name = L"ex_button";
            this->ex_button->Size = System::Drawing::Size(169, 40);
            this->ex_button->TabIndex = 10;
            this->ex_button->Text = L"EXIT";
            this->ex_button->UseVisualStyleBackColor = false;
            this->ex_button->Click += gcnew System::EventHandler(this, &schedule::ex_button_Click);
            // 
            // schedule
            // 
            this->AllowDrop = true;
            this->AutoScroll = true;
            this->AutoSize = true;
            this->AutoSizeMode = System::Windows::Forms::AutoSizeMode::GrowAndShrink;
            this->BackColor = System::Drawing::Color::Black;
            this->ClientSize = System::Drawing::Size(1052, 658);
            this->Controls->Add(this->dataGridViewRound1);
            this->Controls->Add(this->ex_button);
            this->Controls->Add(this->label6);
            this->Controls->Add(this->label5);
            this->Controls->Add(this->label4);
            this->Controls->Add(this->dataGridViewRound2);
            this->Controls->Add(this->dataGridViewRound3);
            this->Controls->Add(this->updateButton);
            this->Controls->Add(this->winnerTextBox);
            this->Controls->Add(this->finalMatchTextBox);
            this->Name = L"schedule";
            this->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &schedule::mouse_Down);
            this->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &schedule::mouse_Move);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridViewRound1))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridViewRound2))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridViewRound3))->EndInit();
            this->ResumeLayout(false);
            this->PerformLayout();

        }
        void ex_button_Click(System::Object^ sender, System::EventArgs^ e)
        {
            this->Close();
        }

#pragma endregion
    
}