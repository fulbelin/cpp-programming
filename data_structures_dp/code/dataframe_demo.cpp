
#include "dataframe.hpp"
using namespace std;


int main() {
  
    // The following should compile    
    //Dataframe df;
    //df.add_column(vector<int>{5,4,3,2,1});
    //cout << df;
    //df.add_column(vector<string>{"A","B","C","D","E"});
    //cout << df;
    //df.set_colnames(vector<string>{"Index", "Category"});
    //cout << df;
    //df.set_rownames(vector<string>{"Sample1", "Sample2", "Sample3", "Sample4", "Sample5"});
    //cout << df;
    //df.swap_columns(0,1);
    //cout << df;
    //df.swap_columns("Category", "Index");
    //cout << df;
    //cout << "Number of rows: " << df.nrows() << endl;
    //cout << "Number of columns: " << df.ncols() << endl;

    //std::function<int(int&)> sqrd = [](int& i) { return i=i*i; };
    //std::function<string(string&)> s_append = [](string& s) { return s +="_ABC"; };

    //df.apply(0, sqrd);
    //cout << df;
    //df.apply(1, s_append);
    //cout << df;
    //cout << "Identity comparison: " << (df == df) << endl;

    //df.clear_rownames();
    //df.clear_colnames();
    //cout << df;

    //Dataframe df2;
    //df2.add_column(vector<int>{5,4,3,2,1});
    //df2.add_column(vector<string>{"A","B","C","D","E"});
    //df2.apply(0, sqrd);
    //df2.apply(1, s_append);
    //cout << df2;
    //cout << "Equality comparison: " << (df == df2) << endl;
    //cout << "Inequality comparison: " << (df != df2) << endl;
    //cout << "Value (2,1): " << df2.get<std::string>(2, 1) << endl;
    //df2.order_by(0);
    //cout << df2 << endl;
    //df2.set_colnames(vector<string>{"Value", "Category"});
    //df2.order_by("Category");
    //cout << df2 << endl;

    return 0;
}