



/// creates dictionary header in file  but doesn't work














#include<bits/stdc++.h>
using namespace std;

struct node{
    node* ptr;
    node* left;
    node* right;
    int datasz;
    const char* data;
    // stack overflow
    bool operator<(const node& n) const{
        string a= data;
        string b= n.data;
       return a < b;
    }
};

//int cnt[256];

string filestring,compressed_string,decompressed_string,extracted_bits="";

map<string,string> H_map;
map<string,int> cnt_map;
map<string,string> code_map;

int dfs(int ind,node* me)
{

    if(me->left == NULL)
    {
        string temp="";
        for(int i=0;i<me->datasz;i++)
            temp+=me->data[i];
        decompressed_string += temp;
        return ind;
    }
    // testing
    if(me->right == NULL)
    {
        cout << "ERROR !!!" << endl;
        return 0;
    }
    int y=0;
    if(extracted_bits[ind]=='1')
        y= dfs(ind+1,me->left);
    else if(extracted_bits[ind]=='0')
        y = dfs(ind+1,me->right);
    else{
        cout << int(extracted_bits[ind]) << "  **  " << ind << endl;
        return 100100;
    }
    return y;
}

void gen_code(node* me,string seq){

    /// leaf
    if(me->left == NULL)
    {
        //cout << "**" << endl;
        //cout << me->data << endl;
        //cout << "leaf" << endl;
        string temp="";
        for(int i=0;i<me->datasz;i++)
            temp+=me->data[i];
        code_map[temp] = seq;
        H_map[seq] = temp;
        //cout << "B" << endl;
        return ;
    }
    // testing
    if(me->right == NULL)
    {
        cout << "ERROR !!!" << endl;
        return ;
    }
    //cout << "L" << endl;
    gen_code(me->left , seq+'1');
    //cout << "R" << endl;
    gen_code(me->right, seq+'0');
    //cout << "B" << endl;
    return ;
}
int needed_bytes(int x)
{
    int bits=0;
    if(!x)
        bits=1;
    else
    {
        while(x)
            x/=2,bits++;
    }
    return (int)ceil(1.0*bits/8);
}

int main()
{
    int nbytes;
    cin >> nbytes;
    FILE *f = fopen("t4.txt", "rb");
    if(!f){
        cout << "File NOT Found" << endl;
        return 0;
    }

    unsigned char c;
    int ic;
    cout << 2 << endl;
    string z="";
    while(fread(&c,sizeof(c),1,f))
    {
        //cout << 1 << endl;
        z+=c;
        filestring+=c;
       // cout << "ic = " << ic << endl;
        //cout << "int(c) = " << int(c) << endl;
        if(z.size()==nbytes)
            cnt_map[z]++ , z="";
        //cout << 3 << endl;
    }
    if(z!="")
        cnt_map[z]++ , z="";
    map<string, int>::iterator it;
    map<string,string>::iterator ite;

    for (it = cnt_map.begin(); it != cnt_map.end(); it++)
    {
        cout << it->first    // string (key)
            << " : "
            << it->second   // string's value
            << endl;
    }

    fclose(f);

    // priority queue min heap  stack_overflow
    priority_queue<pair<int,node>,vector<pair<int,node> >,greater<pair<int,node> > >tree;
    int distinct=0;
    for(it=cnt_map.begin();it!=cnt_map.end();it++)
    {
        node* temp = (node* )malloc(sizeof(node));
        temp->left = NULL;
        temp->right = NULL;
        const char* str = (const char *)malloc((it->first).size());
        str = (it->first).c_str();
        temp->data = str;
        temp->datasz = (it->first).size();
        temp->ptr = temp;
        tree.push(make_pair(it->second,*temp));
    }
    pair<int,node> el = tree.top();
    /*for(int i=0;i<26;i++)
    {
        cout << tree.top().first << " " << tree.top().second.data << endl;
        tree.pop();
    }*/
    distinct = tree.size();
    while(tree.size()>1)
    {
        node* temp = (node* )malloc(sizeof(node));
        temp->left = tree.top().second.ptr;
       // cout << " CHECK !! " << temp->left->data << endl;
        pair<int,node> el;
        el = tree.top();
        tree.pop();
        temp->right = tree.top().second.ptr;
        //cout << " CHECK2 ! " << temp->right->data << endl;
        int freq1 = el.first;
       // cout << "Remove1 : " << el.second.data << " " << "freq1 = " << freq1 << endl;
        /// cast (node *) ???
        el = tree.top();
        tree.pop();
        int freq2 = el.first;
       // cout << "Remove2 : " << el.second.data << " " << "freq2 = " << freq2 << endl;
        /// cast (node *) ???
        temp->data = "-";
        temp->datasz=1;
        temp->ptr = temp;
        node t = *temp;
/*
        cout << endl << endl;
        cout << "ADDt : " << t.data << " " << "freq = " << freq1+freq2 << endl;
        cout << "ADDtemp : " << temp->data << " " << "freq = " << freq1+freq2 << endl;
        cout << "edgest1 : " << "left = " << t.left->data << "   right = " << t.right->data << endl;
        cout << "edgestemp1 : " << "left = " << temp->left->data << "   right = " << temp->right->data << endl;
*/
        tree.push(make_pair(freq1+freq2,t));
/*        cout << "edgest2 : " << "left = " << t.left->data << "   right = " << t.right->data << endl;
        cout << "edgestemp2 : " << "left = " << temp->left->data << "   right = " << temp->right->data << endl;
        cout << endl << endl;
    */}

    /// edge case ???
    el = tree.top();
    cout << "root :  " << tree.size() << " " << el.first  << " " << el.second.data << endl;
    //cout << el.second.left->data << "  kkk  " << el.second.right->data  << endl;
    if(distinct>1)
        gen_code(tree.top().second.ptr,"");
    else
    {
        string temp_else="";
        for(int i=0;i<tree.top().second.ptr->datasz;i++)
            temp_else+=tree.top().second.ptr->data[i];
        code_map[temp_else] = "0";
        H_map["0"] = temp_else;
    }

    cout << endl << endl << "--------------------------------------" << endl << endl;
    int maxcodelen = 0;
    for (ite = code_map.begin(); ite != code_map.end(); ite++)
    {
        cout << ite->first    // string (key)
            << " : "
            << ite->second   // string's value
            << endl;
        maxcodelen = max(maxcodelen,(int)ite->second.size());
    }

    //
    int bits=0;
    for (it = cnt_map.begin(); it != cnt_map.end(); it++)
    {
        int temp = it->second;
        bits += ( code_map[it->first].size() ) * temp;
    }
        cout << endl <<  " TOTAL BITS = " << bits << endl << endl;
    int bit2=0;
    for(ite = code_map.begin();ite!=code_map.end();ite++)
    {
        int temp = cnt_map[ite->first];
        bit2 += int(ite->second.size())*temp;
    }
    cout << bit2 << endl;
    int leftover = bits%8;
    cout << " plz : " << leftover << endl;
    z="";
    for(int i=0;i<filestring.size();i++)
    {
        z+=filestring[i];
        if(z.size()==nbytes)
            compressed_string += code_map[z] , z="";
    }
    if(z!="")
        compressed_string+=code_map[z];

    f = fopen("test.txt","w");
    for(int i=0;i<compressed_string.size();i++)
        c = compressed_string[i] , fputc(c,f);
    fclose(f);
    cout << " Left = " << leftover << endl;
    /*
    f = fopen("extra.txt", "w");
    string z1 ="";
    for(int i=0;i<compressed_string.size();i++)
    {
        z1+=compressed_string[i];
        if (H_map.count(z1)>0)
            fputc(H_map[z1],f) , z1="";
    }
    fclose(f);
    return 0;
    cout << "ASJKD" << endl;
    */


    f = fopen("compressed.bin", "wb");
    if(!f){
        cout << "Error opening file" << endl;
        return 0;
    }
    /*
    save dictionary
    */
    const int var_byte = 4;
    int entries = code_map.size();
    /// ????  fwrite(&var_byte,sizeof(var_byte),1,f);
    fwrite(&nbytes,sizeof(nbytes),1,f);
    fwrite(&var_byte,sizeof(var_byte),1,f);
    fwrite(&entries,sizeof(entries),1,f);
    for (ite = code_map.begin(); ite != code_map.end(); ite++)
    {
        /// write nbytes
        for(int i=0;i<ite->first.size();i++)
        {
            char temp_c = ite->first[i];
            fwrite(&temp_c,sizeof(temp_c),1,f);
        }
        /// write L;
        int l = ite->second.size();
        string str = bitset<var_byte*8>(l).to_string();
        int lefov2 = l%8;
        int byte=0;
        for(int i=0;i<str.size();i++)
        {
            byte += int(str[i]-'0');
            if(i%8==7)
                c = (unsigned char)(byte) , fwrite(&c,sizeof(c),1,f) , byte=0;
            else
                byte*=2;
        }
        byte=0;
        for(int i=0;i<ite->second.size();i++)
        {
            byte += int(ite->second[i]-'0');
            if(i%8==7)
                c = (unsigned char)(byte) , fwrite(&c,sizeof(c),1,f) , byte=0;
            else
                byte*=2;
        }
        if(lefov2)
        {
            c = (unsigned char)byte;
            fwrite(&c,sizeof(c),1,f);
            //fputc((unsigned char)(byte),f);
        }
    }

    //
    c = (unsigned char)leftover;
    fwrite(&c,sizeof(c),1,f);
     //fputc((unsigned char)(leftover),f);
    int byte=0;
    for(int i=0;i<compressed_string.size();i++)
    {
        byte*=2;
        byte += int(compressed_string[i]-'0');
        if(i%8==7)
        {
            c=(unsigned char)byte;
            //fputc((unsigned char)(byte),f);
            fwrite(&c,sizeof(c),1,f);
            byte = 0;
        }
    }
    if(leftover)
    {
        c = (unsigned char)byte;
        fwrite(&c,sizeof(c),1,f);
        //fputc((unsigned char)(byte),f);
    }
    fclose(f);

    f = fopen("com.bin", "rb");
    if(!f){
        return -2;
    }



    //c = fgetc(f);
    fread(&c,sizeof(c),1,f);
    cout << "  cread =  " << (int)c << endl << endl;
    leftover = int(c);
    int no_of_b=0;
    cout << "left 2 = " << leftover << endl;

    //(ic=fgetc(f))!=EOF
    while(fread(&c,sizeof(c),1,f))
    {
        /// n = 1;
        /// hash map
        extracted_bits+= bitset<8>(int(c)).to_string();
        //if(c !=' ')
          //  cout << "-8" << endl;
        //if(int(c)!=0)
          //  cout << "-7" << endl;
        if(extracted_bits.size()<8)
            cout << "-5" << endl;
        no_of_b++;
    }
    cout << " NoOfBytes =  " << no_of_b << endl;
    fclose(f);


    f = fopen("test3.txt","w");
    for(int i=0;i<extracted_bits.size();i++){
        fputc((unsigned char)extracted_bits[i],f);
        if(i%8 ==7)
            fputc('\n',f);
    }

    /// handling left overs   {

    z="";
    for(int i=extracted_bits.size()-leftover;i<extracted_bits.size();i++)
        z+=extracted_bits[i];

    int bits_size = extracted_bits.size();

    if(extracted_bits.size() == 0)
        return -10;
    if(leftover)
    {
        extracted_bits.erase(extracted_bits.begin()+bits_size-8,extracted_bits.end());
        extracted_bits += z;
    }

    ///    }

    /// converting sequential bits to original chars {
    int maxm = 0;
    // infinte loop if root has a charachter "file has only one char"
    cout << "test : " << compressed_string.size() << " " << extracted_bits.size() << endl;
    cout << (int)extracted_bits[extracted_bits.size()] << endl;

    f = fopen("test.txt","w");
    for(int i=0;i<compressed_string.size();i++){
        fputc((unsigned char)compressed_string[i],f);
        if(i%8==7)
            fputc('\n',f);
    }
    fclose(f);
    f = fopen("test2.txt","w");
    for(int i=0;i<extracted_bits.size();i++){
        fputc((unsigned char)extracted_bits[i],f);
        if(i%8 ==7)
            fputc('\n',f);
    }
    fclose(f);

    if(distinct>1)
        for(int i=0;i<extracted_bits.size();i++)
            i = dfs(i,tree.top().second.ptr)-1;
    else
        for(int i=0;i<extracted_bits.size();i++)
            decompressed_string += H_map["0"];


    ///  }

    /// extraction to extra.txt {
    f = fopen("extra.txt", "w");
    if(!f){
        cout << "error";
        return 0;
    }
    for(int i=0;i<decompressed_string.size();i++)
    {
        c = decompressed_string[i];
        fputc(c,f);
    }
    fclose(f);
    /// }

}

