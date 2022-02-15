#include<bits/stdc++.h>
using namespace std;
map<string,string> H_map,code_map;
string extracted_bits , decompressed_string;
int main(){


    FILE *f = fopen("compressed.bin", "rb");
    if(!f){
        return -2;
    }

    unsigned char c;
    int entries,nbytes,var_byte;
    fread(&nbytes,sizeof(nbytes),1,f);
    fread(&var_byte,sizeof(var_byte),1,f);
    fread(&entries,sizeof(entries),1,f);
    for(int k=0;k<entries;k++)
    {
        char temp_c;
        string z="" , zcode="" , extracted_minibits="";
        for(int i=0;i<nbytes;i++)
            fread(&temp_c,sizeof(temp_c),1,f) , z+=temp_c;
        int byte=0;
        for(int i=0;i<var_byte;i++)
            byte *= 256 , fread(&c,sizeof(c),1,f) , byte+=c;
        int l = byte , l2=byte;
        int lefov2 = l%8;

        while(l2>0)
        {
            fread(&c,sizeof(c),1,f);
            extracted_minibits+= bitset<8>(int(c)).to_string();
            l2-=8;
        }
        for(int i=extracted_minibits.size()-lefov2;i<extracted_minibits.size();i++)
            zcode+=extracted_minibits[i];
        int minibits_size = extracted_minibits.size();
        if(lefov2)
        {
            extracted_minibits.erase(extracted_minibits.begin()+minibits_size-8,extracted_minibits.end());
            extracted_minibits += zcode;
        }
        code_map[z] = zcode;
        H_map[zcode] = z;
    }

    fread(&c,sizeof(c),1,f);
    int leftover = int(c);
    while(fread(&c,sizeof(c),1,f))
    {
        extracted_bits+= bitset<8>(int(c)).to_string();
        if(extracted_bits.size()<8)
            cout << "-5" << endl;
    }
    fclose(f);

/*
    f = fopen("test3.txt","w");
    for(int i=0;i<extracted_bits.size();i++){
        fputc((unsigned char)extracted_bits[i],f);
        if(i%8 ==7)
            fputc('\n',f);
    }
*/
    /// handling left overs   {

    string z="";
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
/*
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
*/
    z="";
    for(int i=0;i<extracted_bits.size();i++)
    {
        z+=extracted_bits[i];
        if(H_map.count(z)>0)
            decompressed_string += H_map[z] , z="";
    }
    ///  }

    /// extraction to extra.txt {
    f = fopen("extracted_text.txt", "w");
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
    f = fopen("extracted_binary","wb");
    char temp_c;
    for(int i=0;i<decompressed_string.size();i++)
    {
        temp_c = decompressed_string[i];
        fwrite(&c,sizeof(c),1,f);
    }
    fclose(f);

    /// }

}
