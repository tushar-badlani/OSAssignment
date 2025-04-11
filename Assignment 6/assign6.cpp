#include<bits/stdc++.h>
using namespace std;

int ff_total = 0;
int bf_total = 0;
int nf_total = 0;
int wf_total = 0;

int ff_process_allocated = 0;
int bf_process_allocated = 0;
int nf_process_allocated = 0;
int wf_process_allocated = 0;

void first_fit(int nb, int np, vector<int> block, vector<int> process)
{
    vector<int> vis(np,0);
    for(int i = 0; i < np; i++)
    {
        for(int j = 0; j < nb; j++)
        {
            if(process[i] <= block[j])
            {
            	vis[i] = 1;
                block[j] -= process[i];
                ff_total += block[j];
                cout << "Process " << i+1 << " of size " << process[i] << " is allocated Block No. " << j+1 << " with fragmentation of " << block[j] << endl;
                break;
            }
        }
    }
    
    for(int i=0;i<np;i++)
    {
    	if(!vis[i])
    	{
    		cout<<"Process "<<i+1<<" is not allocated!"<<endl;
    	}
    	else
    	{
    		ff_process_allocated++;
    	}
    }
}

void best_fit(int nb, int np, vector<int> block, vector<int> process)
{
    vector<int> vis(np,0);
    for(int i = 0; i < np; i++)
    {
        int curr_block_idx = -1;
        for(int j = 0; j < nb; j++)
        {
            if(process[i] <= block[j] && (curr_block_idx == -1 || block[j] < block[curr_block_idx]))
            {
                curr_block_idx = j;
            }
        }
        if(curr_block_idx != -1)
        {
            vis[i] = 1;
            block[curr_block_idx] -= process[i];
            bf_total += block[i];
            cout << "Process " << i+1 << " of size " << process[i] << " is allocated Block No. " << curr_block_idx+1 << " with fragmentation of " << block[curr_block_idx] << endl;
        }
    }
    
    for(int i=0;i<np;i++)
    {
    	if(!vis[i])
    	{
    		cout<<"Process "<<i+1<<" is not allocated!"<<endl;
    	}
    	else
    	{
    		bf_process_allocated++;
    	}
    }
}

void next_fit(int nb, int np, vector<int> block, vector<int> process)
{
    vector<int> vis(np,0);
    int j = 0;
    for(int i = 0; i < np; i++)
    {
        int count = 0;
        while(count < nb)
        {
            if(process[i] <= block[j])
            {
                vis[i] = 1;
                block[j] -= process[i];
                nf_total += block[j];
                cout << "Process " << i+1 << " of size " << process[i] << " is allocated Block No. " << j+1 << " with fragmentation of " << block[j] << endl;
                break;
            }
            j = (j + 1) % nb;
            count++;
        }
    }
    
    for(int i=0;i<np;i++)
    {
    	if(!vis[i])
    	{
    		cout<<"Process "<<i+1<<" is not allocated!"<<endl;
    	}
    	else
    	{
    		nf_process_allocated++;
    	}
    }
}

void worst_fit(int nb, int np, vector<int> block, vector<int> process)
{
    vector<int> vis(np,0);
    for(int i = 0; i < np; i++)
    {
        int curr_block_idx = -1;
        for(int j = 0; j < nb; j++)
        {
            if(process[i] <= block[j] && (curr_block_idx == -1 || block[j] > block[curr_block_idx]))
            {
                curr_block_idx = j;
            }
        }
        if(curr_block_idx != -1)
        {
            vis[i] = 1;
            block[curr_block_idx] -= process[i];
            wf_total += block[i];
            cout << "Process " << i+1 << " of size " << process[i] << " is allocated Block No. " << curr_block_idx+1 << " with fragmentation of " << block[curr_block_idx] << endl;
        }
    }
    
    for(int i=0;i<np;i++)
    {
    	if(!vis[i])
    	{
    		cout<<"Process "<<i+1<<" is not allocated!"<<endl;
    	}
    	else
    	{
    		wf_process_allocated++;
    	}
    }
}


int main()
{
    int nb, np;
    cout << "Enter Number of Blocks: " << endl;
    cin >> nb;
    vector<int> block(nb);
    cout << "Enter Size of Each Block: " << endl;
    for(int i = 0; i < nb; i++)
    {
        cin >> block[i];
    }
    cout << "Enter Number of Processes: " << endl;
    cin >> np;
    vector<int> process(np);
    cout << "Enter Size Occupied by Each Process: " << endl;
    for(int i = 0; i < np; i++)
    {
        cin >> process[i];
    }
    int choice;
    cout << "Press 1 for First Fit" << endl;
    cout << "Press 2 for Best Fit" << endl;
    cout << "Press 3 for Next Fit" << endl;
    cout << "Press 4 for Worst Fit" << endl;
    cout << "Press 5 for Analysis of Methods" << endl;

    int yes = 1;
    do{
        cout << "Enter your choice: " << endl;
    	cin >> choice;
    switch(choice)
    {
        case 1:
            first_fit(nb, np, block, process);
            break;
        case 2:
            best_fit(nb, np, block, process);
            break;
        case 3:
            next_fit(nb, np, block, process);
            break;
        case 4:
            worst_fit(nb, np, block, process);
            break;
        case 5:
            worst_fit(nb, np, block, process);
            break;
        default:
            cout << "Enter a valid choice!";
            break;
    }
    cout<<"Do you want to run again: (Yes - 1/No - 0)"<<endl;
    cin>>yes;
    }while(yes);
    return 0;
}

