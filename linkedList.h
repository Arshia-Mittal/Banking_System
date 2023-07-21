#include <iostream>
#include <stack>
#include <string>
#include <pthread.h>
using namespace std;

// node of a linked list represents a bank account
class node
{
public:
    int account_num;
    string name;
    stack<double> st;
    node *next;
    float bal;
    pthread_mutex_t lock;

    // constructor
    node(int n, string s)
    {
        account_num = n;
        name = s;
        next = NULL;
        bal = 0;
        pthread_mutex_init(&lock, NULL);
    }
};

// function to search for a node in the linked list
node *search(node *&head, int acc_no)
{
    if (!head)
    {
        return head;
    }

    node *temp = head;

    while (temp != NULL && temp->account_num != acc_no)
    {
        temp = temp->next;
    }
    return temp;
}

// class bank is linked list
class bank
{
public:
    int s;
    double bank_bal;
    node *head;
    
    // constructor
    bank()
    {
        s = 1;
        bank_bal = 0;
        head = NULL;
    }

    // function to give account details 
    //of all the accounts in the bank
    string details()
    {
        node *temp = head;
        string ans = "";
        while (temp != NULL)
        {
            ans += "account number is: " + to_string(temp->account_num) + " and account holder name is: " + temp->name + "\n";
            temp = temp->next;
        }
        return ans;
    }

    // function to give total bank balance
    double show_bal()
    {
        return bank_bal;
    }

    // function to create a new account
    //(adding new node to linked list)
    int create_account(string name)
    {
        // create a new node
        node *n = new node(-1, name);
        pthread_mutex_lock(&n->lock);
        n->account_num = s;
        int acc_no = s;
        s++;

        // if no node is currently present 
        // in the linked list
        if (!head)
        {
            head = n;
            pthread_mutex_unlock(&n->lock);
            return acc_no;
        }

        // insert new node at the end of the list
        node *temp = head;
        pthread_mutex_lock(&temp->lock);
        node *curr;
        while (temp->next != NULL)
        {
            curr = temp->next;
            pthread_mutex_lock(&curr->lock);
            pthread_mutex_unlock(&temp->lock);
            temp = curr;
        }
        temp->next = n;
        pthread_mutex_unlock(&n->lock);
        pthread_mutex_unlock(&temp->lock);
        return acc_no;
    }

    // function to give current balance of an account
    double balance(int acc_no)
    {
        node *temp = search(head, acc_no);

        // if no such account exists
        if (!temp)
        {
            return -1;
        }

        float ans = 0;
        pthread_mutex_lock(&temp->lock);
        stack<double> t = temp->st;

        // if transaction stack is currently 
        // empty, account balance is 0
        if (t.empty())
        {
            pthread_mutex_unlock(&temp->lock);
            return ans;
        }

        // top of stack is equal to current 
        // balance of the account
        ans = t.top();
        pthread_mutex_unlock(&temp->lock);
        return ans;
    }

    // function to deposit money in the account
    bool deposit(int acc_no, float amount)
    {
        node *temp = search(head, acc_no);

        // if no such account exists
        if (temp == NULL)
        {
            return false;
        }

        float curr_bal = 0;

        pthread_mutex_lock(&temp->lock);
        stack<double> t = temp->st;
        if (!t.empty())
        {
            curr_bal = t.top();
        }
        // push new balance on top of the transaction stack
        temp->st.push(curr_bal + amount);
        pthread_mutex_unlock(&temp->lock);

        // update total bank balance
        bank_bal += amount;
        return true;
    }

    // function to withdraw money from the account
    bool withdraw(int acc_no, float amount)
    {
        node *temp = search(head, acc_no);

        // if no such account exists
        if (!temp)
        {
            return false;
        }

        float curr_bal = 0;

        pthread_mutex_lock(&temp->lock);
        stack<double> t = temp->st;
        if (!t.empty())
        {
            curr_bal = t.top();
        }

        // checks availability of required amount 
        // in the bank account
        if (curr_bal < amount)
        {
            cout << "under balance" << endl;
            pthread_mutex_unlock(&temp->lock);
            return false;
        }

        // push new balance on top of the transaction stack
        temp->st.push(curr_bal - amount);
        pthread_mutex_unlock(&temp->lock);

        // update total bank balance
        bank_bal -= amount;
        return true;
    }

    // function to return account holder's name
    string accHolder(int acc_no)
    {
        node *temp = search(head, acc_no);
        // if no such account exists
        if (!temp)
        {
            return "Nil";
        }
        return temp->name;
    }

    // function to give mini statement of the account
    bool min_statement(int acc_no, vector<double> &v)
    {
        node *temp = search(head, acc_no);

        // if no such account exists
        if (!temp)
        {
            return false;
        }

        pthread_mutex_lock(&temp->lock);
        stack<double> x;
        int i = 1;

        while (!temp->st.empty() && i <= 10)
        {
            v.push_back(temp->st.top());
            x.push(temp->st.top());
            temp->st.pop();
            i++;
        }

        while (!x.empty())
        {
            temp->st.push(x.top());
            x.pop();
        }
        pthread_mutex_unlock(&temp->lock);
        return true;
    }

    // function to reset an account
    bool resetAccount(int acc_no)
    {
        node *temp = search(head, acc_no);
        if (temp == NULL)
        {
            // No such account exists, 
            // thus making a new one
            node *n = new node(acc_no, "Allice");
            pthread_mutex_lock(&n->lock);

            if (!head)
            {
                head = n;
                pthread_mutex_unlock(&n->lock);
                return true;
            }
            else
            {
                node *temp = head;
                pthread_mutex_lock(&temp->lock);
                node *curr;
                while (temp->next != NULL)
                {
                    curr = temp->next;
                    pthread_mutex_lock(&curr->lock);
                    pthread_mutex_unlock(&temp->lock);
                    temp = curr;
                }
                temp->next = n;
                pthread_mutex_unlock(&n->lock);
                pthread_mutex_unlock(&temp->lock);
                return true;
            }
        }

        // making balance of the account zero
        pthread_mutex_lock(&temp->lock);
        if (!temp->st.empty())
        {
            bank_bal -= temp->st.top();
        }
        while (!temp->st.empty())
        {
            temp->st.pop();
        }
        pthread_mutex_unlock(&temp->lock);
        return true;
    }

    // function to delete node from the linked list
    bool remove_account(int acc_no)
    {
        if (!head)
        {
            cout << "No such account exists" << endl;
            return false;
        }

        // if we have to delete head node
        else if (head->account_num == acc_no)
        {
            node *todel = head;
            pthread_mutex_lock(&todel->lock);
            node *temp = head->next;
            pthread_mutex_lock(&temp->lock);
            float curr_bal = 0;
            if (!head->st.empty())
            {
                curr_bal = head->st.top();
            }
            head->st.push(0);
            head = head->next;
            bank_bal -= curr_bal;
            pthread_mutex_unlock(&todel->lock);
            pthread_mutex_unlock(&temp->lock);
            delete (todel);
            return true;
        }

        // to delete node other than head node
        node *temp = head;
        node *curr;
        pthread_mutex_lock(&temp->lock);
        while (temp->next != NULL)
        {
            curr = temp->next;
            pthread_mutex_lock(&curr->lock);
            if (curr->account_num == acc_no)
            {
                float curr_bal = 0;
                if (!curr->st.empty())
                {
                    curr_bal = curr->st.top();
                }
                curr->st.push(0);
                temp->next = curr->next;
                curr->next = NULL;
                bank_bal -= curr_bal;
                pthread_mutex_unlock(&curr->lock);
                pthread_mutex_unlock(&temp->lock);
                delete (curr);
                return true;
            }
            pthread_mutex_unlock(&temp->lock);
            temp = temp->next;
        }
        pthread_mutex_unlock(&temp->lock);
        return false;
    }
};
