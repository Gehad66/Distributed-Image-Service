#include <iostream>
#include <string>
#include <stdio.h>
using namespace std;

namespace steganography{
int get_message_length(const char my_msg[]) {
    int i = 0;
    while(my_msg[i] != '\0') {
        i++;
    }
    return i * 8 + 8;
}
void copy_header(FILE *fp1, int numLines, FILE *fp2) {
    int i;
    char temp;

    rewind(fp1); //Goes back to the beginning of the file

    for(i = 0; i < numLines; i++) {
        while((temp = fgetc(fp1)) != EOF && temp != '\n') {
            fputc(temp, fp2);
        }
        fputc('\n', fp2);
    }

    return;
}

int message_fits(int length, int w, int h) {
    return length < w * h * 3;
}
int count_new_lines(FILE *fp) {
    char temp; int count = 0;

    rewind(fp);

    while((temp = fgetc(fp)) != EOF)
        if(temp == '\n')
            count++;

    return count;
}

void encode_length(FILE *in, FILE *out, int length) {
    char temp;
    int i, l;
    for(i = 0; i < 8; i++) {
        temp = fgetc(in);
        l = length;
        l >>= 7 - i;
        if((l & 1) == 1) {
            if((temp & 1) == 0) {
                temp++;
            }
        } else {
            if((temp & 1) == 1) {
                temp--;
            }
        }
        fputc(temp, out);
    }

    return;
}

void encode_message(FILE *in, FILE *out, int num_to_encode,const char* my_message, int w, int h) {
    int encoded = 0;
    unsigned char temp;
    int idx = 0, num_coppied = 0;
    char current;

    int fileSize = (w * h * 3) - 8; //Number of bytes after first 8
    int i;

    for(i = 0; i < fileSize; i++) {
        temp = fgetc(in);
        current = my_message[idx];

        current >>= 7 - num_coppied;
        num_coppied++;

        if(encoded <= num_to_encode) {
            encoded++;
            if((current & 1) == 1) {
                if((temp & 1) == 0) {
                    temp++;
                }
            } else {
                if((temp & 1) == 1) {
                    temp--;
                }
            }
            if(num_coppied == 8) {
                idx++;
                num_coppied = 0;
            }
        }

        fputc(temp, out);
    }

    return;
}
int read_ppm_type(FILE *fp) {
    char temp = fgetc(fp);
    cout << "Inr eading ppm type, temp is " << temp << endl;
    if(temp == 'P' && fgetc(fp) == '6') {
        fgetc(fp);
            cout << "Inr eading ppm type, fp is " << fp << endl;

        return 1;
    }

    return 0;
}
void skip_comments(FILE *fp) {
    char temp;
    while((temp = fgetc(fp)) == '#') {
        while(fgetc(fp) != '\n') {}
    }

    //Give back the first character of width that we took
    ungetc(temp, fp);

    return;
}
int get_width(FILE *fp) {
    int w;
    fscanf(fp, "%d", &w);
    return w;
}
int get_height(FILE *fp) {
    int h;
    fscanf(fp, "%d", &h);
    return h;
}
int read_color_depth(FILE *fp) {
    int c;
    fscanf(fp, "%d", &c);
    if(c == 255) {
        fgetc(fp);    //Get rid of the trailing whitespace
        return 1;
    }
    return 0;
}

void WriteMessageToPic(string image_name, string message, string write_image_name) {

    FILE *fp;
    if((fp = fopen(image_name.c_str(), "r+")) == NULL) {
        printf("Could not open file %s.\nAborting\n", "hackny.ppm");
        return;
    }

    if(read_ppm_type(fp)) {
        skip_comments(fp);

        const char* myMessage = message.c_str();
        int message_length = get_message_length(myMessage);
        int w = get_width(fp);
        int h = get_height(fp);

        if(message_fits(message_length, w, h)) {
            if(read_color_depth(fp)) {
                FILE *fp_t = fopen(write_image_name.c_str(),"w");
                int i = count_new_lines(fp);
                copy_header(fp, i, fp_t);
                encode_length(fp, fp_t, (message_length - 8) / 8);
                encode_message(fp, fp_t, (message_length - 8), myMessage, w, h);
                printf("Encoding Process Complete. Take a look!\n");

                fclose(fp);
                fclose(fp_t);
            } else {
                printf("\nError: Image color depth invalid. Must be 255.\n");
                return;
            }
        } else {
            printf("\nError: Image file is not large enough to hold the message.\nAborting.\n");
            return;
        }
    } else {
        printf("Error: Wrong file format.\nAborting\n");
        return;
    }

    return;
}

int get_msg_length(FILE *fp) {
    char temp = 0;
    int length = 0;
    int i;

    for(i=0; i < 8; i++) {
        temp = fgetc(fp);
        //Delay the shift by one to get the 29, otherwise I'd get 58
        if(i > 0) length <<= 1;
        length |= (temp & 1);
    }

    return length;
}

string decode_message(int length, FILE *fp) {
    int readTo = length * 8, numRead = 0, i;
    unsigned char charBuffer = 0;
    char temp;
    char secret_message[length + 1];
    int idx = 0;

    while(numRead <= readTo) {
        temp = fgetc(fp);
        if(numRead % 8 == 0) {
            secret_message[idx] = charBuffer;
            idx++;
            charBuffer = 0;
        } else {
            charBuffer <<= 1;
        }
        charBuffer |= (temp & 1);
        numRead++;
    }//end while

    //Start printing from character 1 because the first char is junk
    for(i = 1; i < idx; i++) {
        printf("%c", secret_message[i]);
    }

    string ans;
    for (int i = 0; i < idx; ++i) {
    ans += string(1, secret_message[i]);
    }

      printf("\n\n");
    cout<<"printing in stego"<<ans<<endl;
      return ans;
}

string ReadMessageFromPic(string image_name) {
    FILE *fp;
string ans="";

    if((fp = fopen(image_name.c_str(), "rb")) == NULL) {
        printf("\ncouldn't open the image\n\n");
        return string("error");
    }

    if(read_ppm_type(fp)) {
        skip_comments(fp);
        get_width(fp);
        get_height(fp);
        if(read_color_depth(fp)) {
            int length = get_msg_length(fp);

            printf("\nHoly secret message batman! We cracked the code: \n");
          ans=  decode_message(length, fp);

            fclose(fp);

        } else {
            printf("Error: Invalid Color Depth.");
            return string("error");
        }
    } else {
        printf("Error: Wrong PPM File Format. Terminating.");
        return string("error");
    }

    return ans;
}

}
