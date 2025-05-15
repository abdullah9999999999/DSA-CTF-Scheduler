#pragma once
#include <fstream>  // For file handling
using namespace std;
namespace DSP {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;
    using namespace std;  // Use standard namespace

  
    class TeamNode { // Binary tree node for teams
    public:
        char teamName[50];
        int members;
        int rank;
        TeamNode* left;
        TeamNode* right;

        TeamNode(const char* name, int mem, int rk) {
           
            int i = 0;
            while (name[i] != '\0' && i < 49) {
                teamName[i] = name[i];
                i++;
            }
            teamName[i] = '\0'; // Ensure null-termination
            members = mem;
            rank = rk;
            left = nullptr;
            right = nullptr;
        }
    };

    public ref class teams : public System::Windows::Forms::Form
    {
    
    public:
        Point mouseLocation;
        teams(void)
        {
            InitializeComponent();
            LoadTeamData();

        }


    protected:
        ~teams()
        {
            if (components)
            {
                delete components;
            }
        }

    private:
        System::Windows::Forms::DataGridView^ groupAView;
        System::Windows::Forms::DataGridView^ groupBView;
        System::ComponentModel::Container^ components;
    private: System::Windows::Forms::Label^ label1;
    private: System::Windows::Forms::Label^ label2;
    private: System::Windows::Forms::Button^ back_button;

           TeamNode* root = nullptr; // Root of the binary tree
    private:
        // Insert a node into the binary tree
        TeamNode* InsertNode(TeamNode* node, const char* name, int members, int rank)
        {
            if (node == nullptr) {
                return new TeamNode(name, members, rank);
            }
            if (rank < node->rank) {
                node->left = InsertNode(node->left, name, members, rank);
            }
            else {
                node->right = InsertNode(node->right, name, members, rank);
            }
            return node;
        }

        // Perform in-order traversal and populate the DataGridViews
        void InOrderTraversal(TeamNode* node, System::Windows::Forms::DataGridView^ groupAView, System::Windows::Forms::DataGridView^ groupBView)
        {
            if (node == nullptr) return;

            // Traverse the left subtree
            InOrderTraversal(node->left, groupAView, groupBView);

            // Add to Group A or Group B based on rank
            if (node->rank <= 8) {
                groupAView->Rows->Add(gcnew String(node->teamName), node->members.ToString(), node->rank.ToString());
            }
            else {
                groupBView->Rows->Add(gcnew String(node->teamName), node->members.ToString(), node->rank.ToString());
            }

            // Traverse the right subtree
            InOrderTraversal(node->right, groupAView, groupBView);
        }

        // Load the team data from the file and populate the binary tree
        void LoadTeamData()
        {
            // Configure the columns for both DataGridViews
            ConfigureGridView(groupAView, "Group A");
            ConfigureGridView(groupBView, "Group B");

            fstream file("E:\\CTFdata.txt", ios::in);
            if (!file.is_open()) {
                MessageBox::Show("Error opening file!", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
                return;
            }

            char line[64]; // Buffer to read each line
            while (file.getline(line, sizeof(line)))
            {
                // analyze the line manually without strtok, using basic parsing logic
                char* teamName = line;
                char* membersStr = nullptr;
                char* rankStr = nullptr;

                // Find ':' and split the string
                for (int i = 0; line[i] != '\0'; i++) {
                    if (line[i] == ':') {
                        line[i] = '\0';  // Null terminate the team name
                        membersStr = &line[i + 1]; // Members string starts after ':'
                        break;
                    }
                }

                // Find the comma between members and rank
                for (int i = 0; membersStr && membersStr[i] != '\0'; i++) {
                    if (membersStr[i] == ',') {
                        membersStr[i] = '\0';  // Null terminate the members string
                        rankStr = &membersStr[i + 1]; // Rank string starts after ','
                        break;
                    }
                }

                if (teamName && membersStr && rankStr) {
                    int members = 0;
                    for (int i = 0; membersStr[i] != '\0'; i++) {
                        members = members * 10 + (membersStr[i] - '0'); // Convert members string to int
                    }

                    int rank = 0;
                    for (int i = 0; rankStr[i] != '\0'; i++) {
                        rank = rank * 10 + (rankStr[i] - '0'); // Convert rank string to int
                    }

                    // Insert into the binary tree
                    root = InsertNode(root, teamName, members, rank);
                }
            }

            file.close();

            // Populate the DataGridViews using in-order traversal
            InOrderTraversal(root, groupAView, groupBView);
        }

        // Configure a DataGridView
        void ConfigureGridView(System::Windows::Forms::DataGridView^ gridView, String^ groupName)
        {
            gridView->ColumnCount = 3;
            gridView->Columns[0]->Name = "Team Name";
            gridView->Columns[1]->Name = "Members";
            gridView->Columns[2]->Name = "Rank";
        }
    private: System::Void back_button_Click(System::Object^ sender, System::EventArgs^ e) {
        this->Hide();
    }
    private: System::Void groupBView_CellContentClick(System::Object^ sender, System::Windows::Forms::DataGridViewCellEventArgs^ e) {
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
               this->groupAView = (gcnew System::Windows::Forms::DataGridView());
               this->groupBView = (gcnew System::Windows::Forms::DataGridView());
               this->label1 = (gcnew System::Windows::Forms::Label());
               this->label2 = (gcnew System::Windows::Forms::Label());
               this->back_button = (gcnew System::Windows::Forms::Button());
               (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->groupAView))->BeginInit();
               (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->groupBView))->BeginInit();
               this->SuspendLayout();
               // 
               // groupAView
               // 
               this->groupAView->AccessibleRole = System::Windows::Forms::AccessibleRole::None;
               this->groupAView->AllowDrop = true;
               this->groupAView->AllowUserToAddRows = false;
               this->groupAView->AllowUserToDeleteRows = false;
               this->groupAView->AllowUserToResizeRows = false;
               this->groupAView->BackgroundColor = System::Drawing::Color::Black;
               dataGridViewCellStyle1->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
               dataGridViewCellStyle1->BackColor = System::Drawing::Color::Black;
               dataGridViewCellStyle1->Font = (gcnew System::Drawing::Font(L"Agency FB", 10, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
                   static_cast<System::Byte>(0)));
               dataGridViewCellStyle1->ForeColor = System::Drawing::Color::White;
               dataGridViewCellStyle1->SelectionBackColor = System::Drawing::Color::Black;
               dataGridViewCellStyle1->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
               dataGridViewCellStyle1->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
               this->groupAView->ColumnHeadersDefaultCellStyle = dataGridViewCellStyle1;
               this->groupAView->ColumnHeadersHeight = 34;
               dataGridViewCellStyle2->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
               dataGridViewCellStyle2->BackColor = System::Drawing::Color::Black;
               dataGridViewCellStyle2->Font = (gcnew System::Drawing::Font(L"Agency FB", 10, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
                   static_cast<System::Byte>(0)));
               dataGridViewCellStyle2->ForeColor = System::Drawing::Color::Black;
               dataGridViewCellStyle2->SelectionBackColor = System::Drawing::Color::Black;
               dataGridViewCellStyle2->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
               dataGridViewCellStyle2->WrapMode = System::Windows::Forms::DataGridViewTriState::False;
               this->groupAView->DefaultCellStyle = dataGridViewCellStyle2;
               this->groupAView->GridColor = System::Drawing::Color::Black;
               this->groupAView->Location = System::Drawing::Point(24, 88);
               this->groupAView->Name = L"groupAView";
               this->groupAView->ReadOnly = true;
               dataGridViewCellStyle3->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
               dataGridViewCellStyle3->BackColor = System::Drawing::Color::Black;
               dataGridViewCellStyle3->Font = (gcnew System::Drawing::Font(L"Agency FB", 10, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
                   static_cast<System::Byte>(0)));
               dataGridViewCellStyle3->ForeColor = System::Drawing::Color::White;
               dataGridViewCellStyle3->SelectionBackColor = System::Drawing::Color::Black;
               dataGridViewCellStyle3->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
               dataGridViewCellStyle3->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
               this->groupAView->RowHeadersDefaultCellStyle = dataGridViewCellStyle3;
               this->groupAView->RowHeadersVisible = false;
               this->groupAView->RowHeadersWidth = 62;
               this->groupAView->RowTemplate->DefaultCellStyle->BackColor = System::Drawing::Color::Black;
               this->groupAView->RowTemplate->DefaultCellStyle->Font = (gcnew System::Drawing::Font(L"Agency FB", 10, System::Drawing::FontStyle::Bold,
                   System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
               this->groupAView->RowTemplate->DefaultCellStyle->ForeColor = System::Drawing::Color::White;
               this->groupAView->RowTemplate->DefaultCellStyle->SelectionBackColor = System::Drawing::Color::Black;
               this->groupAView->Size = System::Drawing::Size(530, 485);
               this->groupAView->TabIndex = 0;
               // 
               // groupBView
               // 
               this->groupBView->AccessibleRole = System::Windows::Forms::AccessibleRole::None;
               this->groupBView->AllowDrop = true;
               this->groupBView->AllowUserToAddRows = false;
               this->groupBView->AllowUserToDeleteRows = false;
               this->groupBView->AllowUserToResizeRows = false;
               this->groupBView->BackgroundColor = System::Drawing::Color::Black;
               dataGridViewCellStyle4->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
               dataGridViewCellStyle4->BackColor = System::Drawing::Color::Black;
               dataGridViewCellStyle4->Font = (gcnew System::Drawing::Font(L"Agency FB", 10, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
                   static_cast<System::Byte>(0)));
               dataGridViewCellStyle4->ForeColor = System::Drawing::Color::White;
               dataGridViewCellStyle4->SelectionBackColor = System::Drawing::Color::Black;
               dataGridViewCellStyle4->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
               dataGridViewCellStyle4->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
               this->groupBView->ColumnHeadersDefaultCellStyle = dataGridViewCellStyle4;
               this->groupBView->ColumnHeadersHeight = 34;
               dataGridViewCellStyle5->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleLeft;
               dataGridViewCellStyle5->BackColor = System::Drawing::Color::Black;
               dataGridViewCellStyle5->Font = (gcnew System::Drawing::Font(L"Agency FB", 10, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
                   static_cast<System::Byte>(0)));
               dataGridViewCellStyle5->ForeColor = System::Drawing::Color::Black;
               dataGridViewCellStyle5->SelectionBackColor = System::Drawing::Color::Black;
               dataGridViewCellStyle5->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
               dataGridViewCellStyle5->WrapMode = System::Windows::Forms::DataGridViewTriState::False;
               this->groupBView->DefaultCellStyle = dataGridViewCellStyle5;
               this->groupBView->GridColor = System::Drawing::Color::Black;
               this->groupBView->Location = System::Drawing::Point(589, 88);
               this->groupBView->Name = L"groupBView";
               this->groupBView->ReadOnly = true;
               this->groupBView->RowHeadersVisible = false;
               this->groupBView->RowHeadersWidth = 62;
               this->groupBView->RowTemplate->DefaultCellStyle->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
               this->groupBView->RowTemplate->DefaultCellStyle->BackColor = System::Drawing::Color::Black;
               this->groupBView->RowTemplate->DefaultCellStyle->Font = (gcnew System::Drawing::Font(L"Agency FB", 10, System::Drawing::FontStyle::Bold,
                   System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
               this->groupBView->RowTemplate->DefaultCellStyle->ForeColor = System::Drawing::Color::White;
               this->groupBView->RowTemplate->DefaultCellStyle->SelectionBackColor = System::Drawing::Color::Black;
               this->groupBView->Size = System::Drawing::Size(530, 485);
               this->groupBView->TabIndex = 1;
               this->groupBView->CellContentClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &teams::groupBView_CellContentClick);
               // 
               // label1
               // 
               this->label1->AutoSize = true;
               this->label1->Font = (gcnew System::Drawing::Font(L"Kabisat Demo Tall", 18, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
                   static_cast<System::Byte>(0)));
               this->label1->ForeColor = System::Drawing::Color::White;
               this->label1->Location = System::Drawing::Point(144, 8);
               this->label1->Name = L"label1";
               this->label1->Size = System::Drawing::Size(280, 77);
               this->label1->TabIndex = 2;
               this->label1->Text = L"GROUP A";
               // 
               // label2
               // 
               this->label2->AutoSize = true;
               this->label2->Font = (gcnew System::Drawing::Font(L"Kabisat Demo Tall", 18, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
                   static_cast<System::Byte>(0)));
               this->label2->ForeColor = System::Drawing::Color::White;
               this->label2->Location = System::Drawing::Point(725, 9);
               this->label2->Name = L"label2";
               this->label2->Size = System::Drawing::Size(281, 77);
               this->label2->TabIndex = 3;
               this->label2->Text = L"GROUP B";
               // 
               // back_button
               // 
               this->back_button->BackColor = System::Drawing::Color::White;
               this->back_button->FlatAppearance->BorderColor = System::Drawing::Color::Black;
               this->back_button->FlatAppearance->BorderSize = 2;
               this->back_button->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
               this->back_button->Font = (gcnew System::Drawing::Font(L"Kabisat Demo Tall", 8, System::Drawing::FontStyle::Italic, System::Drawing::GraphicsUnit::Point,
                   static_cast<System::Byte>(0)));
               this->back_button->ForeColor = System::Drawing::Color::Black;
               this->back_button->Location = System::Drawing::Point(478, 592);
               this->back_button->Name = L"back_button";
               this->back_button->Size = System::Drawing::Size(210, 45);
               this->back_button->TabIndex = 6;
               this->back_button->Text = L"Back";
               this->back_button->UseVisualStyleBackColor = false;
               this->back_button->Click += gcnew System::EventHandler(this, &teams::back_button_Click);
               // 
               // teams
               // 
               this->AutoScaleDimensions = System::Drawing::SizeF(9, 20);
               this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
               this->BackColor = System::Drawing::Color::Black;
               this->ClientSize = System::Drawing::Size(1143, 663);
               this->Controls->Add(this->back_button);
               this->Controls->Add(this->label2);
               this->Controls->Add(this->label1);
               this->Controls->Add(this->groupAView);
               this->Controls->Add(this->groupBView);
               this->ForeColor = System::Drawing::Color::Black;
               this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
               this->Name = L"teams";
               this->Text = L"Teams";
               this->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &teams::mouse_Down);
               this->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &teams::mouse_Move);
               (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->groupAView))->EndInit();
               (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->groupBView))->EndInit();
               this->ResumeLayout(false);
               this->PerformLayout();

           }
#pragma endregion

}
