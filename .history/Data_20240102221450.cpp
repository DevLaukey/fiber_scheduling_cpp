// Data.cpp

int *get_data()
{
    static int shared_data = 0; // Initialize shared data
    return &shared_data;
}
