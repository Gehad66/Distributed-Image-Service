/* Be careful not to send parameters to threads by reference!
 */

#include <algorithm>
#include <fstream>
#include <unistd.h>
#include <stdio.h>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sstream>
#include <regex>

#include <QMessageBox>
#include <iostream>
#include <map>
#include <string>
#include <ctime>
#include <algorithm>
#include "peer.h"
#include "ui_peer.h"
#include "../message.h"
#include <QPixmap>
#include <QFile>
#include <QTextStream>
#include "steganography.cc"

using namespace  std;
// Construction!
const std::string Peer::IMAGES_RECEIVED_FILE_NAME = "receivedImages.txt";
Peer::Peer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Peer)
{
    ui->setupUi(this);
    this->set_bind_widgets_visibility(true);
    this->set_sign_in_widgets_visibility(false);
    this->set_sign_out_widgets_visibility(false);
    this->set_send_widgets_visibility(false);
    this->list_image_files_thread = async(launch::async, &Peer::reading_list_images, this);


}
void Peer::reading_list_images(){
    while(1){
        usleep(1000000*50);
        QString path = QString::fromUtf8(IMAGES_RECEIVED_FILE_NAME.c_str());
        QFile inputFile(path);

        if(inputFile.open(QIODevice::ReadOnly)){
            QTextStream in(&inputFile);
            while(!in.atEnd()){
                QString line= in.readLine();
                ui->images_list_widget->addItem(line);
            }
            inputFile.close();
        }
        else{
            std::cout<<"error in opening the file to read from it"<<std::endl;
        }
    }

}

Peer::~Peer()
{
    Message sign_out_request(SignOutRequest);
    sign_out_request.set_username(this->username);
    this->socket->send(sign_out_request,
                       this->server_ip.c_str(),
                       this->server_port);
    delete this->ui;
    delete this->socket;
}

// Auxiliary funcions!
string Peer::get_string_from_line_edit(const QLineEdit * _line_edit, const bool _strict)
{
    /* Args:
     * line_edit: LineEdit Object to retrieve text from.
     * strict: If true, the LineEdit object must be nonempty.
     */
    string ans = _line_edit->text().toStdString();
    if (_strict && ans.empty())
    {
        throw std::runtime_error("Unexpected empty LineEdit! Did you forget to fill in some needed information?");
    }
    return ans;
}

void Peer::show_message_box(const string & message_content, const string & title)
{
    QMessageBox::information(
                this,
                QString::fromStdString(title),
                QString::fromStdString(message_content));
}
void Peer::write_text_file(const string & str, const string & file_name)
{
    ofstream sout(file_name);
    if (sout.fail()) {
        throw runtime_error("nope. output file failed");
    }

    sout << str;
}
// Visibility modifiers!
void Peer::set_widgets_visibility(const vector<QWidget*> _widgets, const bool _visibility)
{
    for (auto widget : _widgets)
    {
        widget->setVisible(_visibility);
    }
}

void Peer::set_sign_in_widgets_visibility(const bool _visibility)
{
    this->set_widgets_visibility(
    {
                    ui->sign_in_push_button,
                    ui->sign_up_push_button,
                    ui->username_line_edit,
                    ui->password_line_edit,
                    ui->server_ip_line_edit,
                    ui->server_port_line_edit
                },
                _visibility);
}

void Peer::set_bind_widgets_visibility(const bool _visibility)
{
    this->set_widgets_visibility(
    {
                    ui->bind_push_button,
                    ui->local_port_line_edit
                },
                _visibility);
}

void Peer::set_sign_out_widgets_visibility(bool _visibility)
{
    this->set_widgets_visibility(
    {
                    ui->sign_out_push_button
                },
                _visibility);
}

void Peer::set_send_widgets_visibility(bool _visibility)
{
    this->set_widgets_visibility(
    {
                    ui->send_push_button,
                    ui->image_id_line_edit,
                    ui->receiver_ip_line_edit,
                    ui->receiver_port_line_edit,
                    ui->get_online_push_button,
                    ui->show_image_push_button,
                    ui->images_list_widget,
                    ui->request_quota_push_button
                },
                _visibility);
}

// Events!
void Peer::on_bind_push_button_clicked()
{
    const int local_port_number = atoi(get_string_from_line_edit(ui->local_port_line_edit).c_str());
    try {
        this->socket = new Socket(local_port_number);
    }
    catch (const exception & e)
    {
        this->show_message_box(e.what());
        return;
    }
    this->set_bind_widgets_visibility(false);
    this->set_sign_in_widgets_visibility(true);
    this->show_message_box("Bind successful!");
}

void Peer::on_sign_up_push_button_clicked()
{
    Message sign_up_request(SignUpRequest);
    const string username = get_string_from_line_edit(ui->username_line_edit);
    const string password = get_string_from_line_edit(ui->password_line_edit);
    const string server_ip = get_string_from_line_edit(ui->server_ip_line_edit);
    const int server_port = atoi(get_string_from_line_edit(ui->server_port_line_edit).c_str());
    sign_up_request.set_username(username);
    sign_up_request.set_password(password);
    this->socket->send(sign_up_request, server_ip.c_str(), server_port);
    
    const auto received = this->socket->receive();
    const auto & reply = received.first;
    const auto & address = received.second;
    
    switch(reply.get_rpc_id())
    {
    case SignUpConfirmation:
        this->show_message_box("Sign Up Successful!");
        break;
    case Error:
        this->show_message_box("Authentication error: " + reply.get_error_message());
        break;
    default:
        this->show_message_box("Unexpected message received!");
        break;
    }
}

void Peer::on_sign_in_push_button_clicked()
{
    // Get sign-in information from widgets!
    const string username = get_string_from_line_edit(ui->username_line_edit);
    const string password = get_string_from_line_edit(ui->password_line_edit);
    const string server_ip = get_string_from_line_edit(ui->server_ip_line_edit);
    const int server_port = atoi(get_string_from_line_edit(ui->server_port_line_edit).c_str());
    
    // Request sign-in!
    Message sign_in_request(SignInRequest);
    sign_in_request.set_username(username);
    sign_in_request.set_password(password);
    this->socket->send(sign_in_request, server_ip.c_str(), server_port);
    
    // Wait for reply!
    const auto received = this->socket->receive();
    const auto & reply = received.first;
    const auto & address = received.second;
    
    switch(reply.get_rpc_id())
    {
    case SignInConfirmation:
        this->signed_in = true;
        this->username = username;
        this->server_ip = server_ip;
        this->server_port = server_port;
        this->set_sign_in_widgets_visibility(false);
        this->set_send_widgets_visibility(true);
        this->set_sign_out_widgets_visibility(true);
        this->show_message_box("Sign in successful!");
        this->listening_thread = async(launch::async, &Peer::listen, this);
        break;
    case Error:
        this->show_message_box("Authentication error: " + reply.get_error_message());
        break;
    default:
        this->show_message_box("Unexpected message received! Was Expecting SignInConfirmation!");
        break;
    }
}

void Peer::on_sign_out_push_button_clicked()
{
    Message sign_out_request(SignOutRequest);
    sign_out_request.set_username(this->username);
    this->socket->send(
                sign_out_request,
                this->server_ip.c_str(),
                this->server_port);
    this->signed_in = false;
    this->username = "";
    this->server_ip = "";
    this->server_port = 0;
    this->set_sign_out_widgets_visibility(false);
    this->set_send_widgets_visibility(false);
    this->set_sign_in_widgets_visibility(true);
    this->show_message_box("Sign out successful!");
    exit(0);
}
void Peer::on_show_image_push_button_clicked()
{
    //      steganography::ReadMessageFromPic(write_image_name);
    QListWidgetItem *item = ui->images_list_widget->currentItem();
    QString image_name = item->text();

    QString path = image_name;
    QPixmap myPixmap;
    bool allow_show=allow_image_display(image_name.toStdString());
    //allow_show = true;
    cout << "Is user allowed? \n" << allow_show << endl;
   // if(!allow_show) return;

    if(allow_show){
    if(!myPixmap.load(path)){
        qWarning("failed to load");
    } else {
        int w = ui->image_label->width();
        int h = ui->image_label->height();
        ui->image_label->setPixmap(myPixmap.scaled(w,h,Qt::KeepAspectRatio));
    }
}
    else{
              QString path_div = QString::fromUtf8(string("dan.png").c_str());
             if(!myPixmap.load(path_div)){
                 qWarning("failed to load");
             }
             else{

                 int w = ui->image_label->width();
                 int h = ui->image_label->height();
                 ui->image_label->setPixmap(myPixmap.scaled(w,h,Qt::KeepAspectRatio));

             }

         }

}

void Peer::on_get_online_push_button_clicked()
{
    Message view_registry_request(ViewRegistryRequest);
    this->socket->send(view_registry_request, this->server_ip.c_str(), this->server_port);
}
void Peer::on_send_push_button_clicked()
{
    string write_image_name = "en_"+get_string_from_line_edit(ui->image_id_line_edit);
    string encoded_msg="Amr 5;Bish 6;Geh 7;"; //Temp syntax
    encoded_msg = encoded_msg + this->username + " -1;"; //give the owner -1 counts(logically means inifinity);
    steganography:: WriteMessageToPic(get_string_from_line_edit(ui->image_id_line_edit),encoded_msg,write_image_name);

    this->chunk_and_send_image(write_image_name,//get_string_from_line_edit(ui->image_id_line_edit),
                               get_string_from_line_edit(ui->receiver_ip_line_edit),
                               atoi(get_string_from_line_edit(ui->receiver_port_line_edit).c_str()));
}

// Sending images!
std::string Peer::file_to_string(const std::string & _file_path){
    std::ifstream input_file;
    input_file.open(_file_path.c_str(), std::ios::in|std::ios::binary|std::ios::ate);
    std::ifstream::pos_type size = input_file.tellg();
    input_file.seekg(0, std::ios::beg);
    char * data = new char[size];
    input_file.read(data, size);
    std::string ans;
    for (int i = 0; i < size; ++i) ans += std::string(1, data[i]);
    delete data;
    input_file.close();
    return ans;
}
void Peer::string_to_file(const std::string & _file_path, const std::string & _file_content){
    std::ofstream output_file;
    output_file.open(_file_path.c_str(), std::ofstream::binary);
    output_file.write(_file_content.c_str(), _file_content.length());
    output_file.close();
}

void Peer::save_image_name_DB(string img_Name) //save to file every 1 min
{

    std::ofstream users_file;
    // std::ofstream ofs("name.txt", std::ios::ate);

    //ofstream log;
    //log.open("log.txt", ofstream::app);

    users_file.open(IMAGES_RECEIVED_FILE_NAME,ofstream::app);
    // this->users_mutex.lock();

    users_file <<img_Name << endl;

    users_file.close();
    // this->log("Successfully saved " + std::to_string(this->users.size())  + " users information to file!");
    //this->users_mutex.unlock();

}

void Peer::chunk_and_send_image(const std::string _image_id, const std::string _receiver_ip, const int _receiver_port) {
    const std::string image_data = Peer::file_to_string(_image_id);
    const int num_chunks = image_data.length() / Peer::MAX_IMAGE_CHUNK_SIZE
            + bool(image_data.length() % Peer::MAX_IMAGE_CHUNK_SIZE);
    std::vector<Message> ans;
    for (int i = 0; i < num_chunks; ++i)
    {
        Message image_chunk(ImageChunk);
        image_chunk.set_image_id(_image_id);
        image_chunk.set_image_chunk_index(i);
        image_chunk.set_image_num_chunks(num_chunks);
        image_chunk.set_image_chunk_content(image_data.substr(i * Peer::MAX_IMAGE_CHUNK_SIZE, Peer::MAX_IMAGE_CHUNK_SIZE));
        // usleep(10000);
        std::cout << "Sending packet of " << i << std::endl;
        this->socket->send(image_chunk, _receiver_ip.c_str(), _receiver_port);
    }
}


void  Peer::send_one_image_chunk(const std::string _image_id, const std::string _receiver_ip, const int _receiver_port, int _chunck_index_to_send) {
    const std::string image_data =  file_to_string(_image_id);
    const int num_chunks = image_data.length() /  MAX_IMAGE_CHUNK_SIZE
            + bool(image_data.length() %  MAX_IMAGE_CHUNK_SIZE);
    std::vector<Message> ans;
    Message image_chunk(ImageChunk);
    image_chunk.set_image_id(_image_id);
    image_chunk.set_image_chunk_index(_chunck_index_to_send);
    image_chunk.set_image_num_chunks(num_chunks);
    image_chunk.set_image_chunk_content(image_data.substr(_chunck_index_to_send *  MAX_IMAGE_CHUNK_SIZE,  MAX_IMAGE_CHUNK_SIZE));
    this->socket->send(image_chunk, _receiver_ip.c_str(), _receiver_port);
    
}
void  Peer::send_one_image_chunk_request(const std::string _image_id, const std::string _receiver_ip, const int _receiver_port, int _chunck_index_to_send) {

    Message image_chunk(oneChunkRequested);
    image_chunk.set_image_id(_image_id);
    image_chunk.set_image_chunk_index(_chunck_index_to_send);
    this->socket->send(image_chunk, _receiver_ip.c_str(), _receiver_port);

}

//receives & saves if complete receival happend
void Peer::serve(std::pair<Message, sockaddr_in> _received)
{
    //const auto &
    Message message = _received.first;
    const auto & address = _received.second;
    switch (message.get_rpc_id()) {
    case ImageChunk:
    {
        this->buffer_mutex.lock();
        std::cout<<"saving packets"<<std::endl;
        std::string ip_and_port_concat=std::string(inet_ntoa(address.sin_addr))+std::string("*")+std::string(to_string(ntohs(address.sin_port)));
        const auto key = make_pair(
                    ip_and_port_concat, message.get_image_id());

        if(image_buffer2.find(key) == image_buffer2.end())
        {
            time_t time2;  time2=time(0);
            image_buffer2.insert(
                        make_pair( key,
                                   make_pair(time2, map<int32_t, Message>())
                                   ) );
            image_buffer2.find(key)->second.second.insert(make_pair(message.get_image_chunk_index(), message));
        }
        else{
            image_buffer2.find(key)->second.second.insert(make_pair(message.get_image_chunk_index(), message));
        }

        if(image_buffer2.find(key)->second.second.size()==message.get_image_num_chunks()){
            string image_data = "";
            map<int32_t, Message> temp=image_buffer2.find(key)->second.second;
            for ( int i=0;i<message.get_image_num_chunks();i++)
            {
                image_data += temp.find(i)->second.get_image_chunk_content();
            }
            std::cout<<"size 2bl delete"<<image_buffer2.size()<<std::endl;

            string_to_file("received_" + message.get_image_id(), image_data);
            save_image_name_DB( ( "received_" + message.get_image_id() ) );
            auto itr2=image_buffer2.find(key);
            image_buffer2.erase(itr2);
            std::cout<<"size after delete"<<image_buffer2.size()<<std::endl;
            std::cout<<"Save Success inside SERVE  fun \n";
        }
        this->buffer_mutex.unlock();
    }
        break;
    case oneChunkRequested:
    {

        //send ?!!!
        //🚨🚨🚨🚨🚨🚨🚨🚨🚨🚨🚨 usually received but we're gonna send, what abt spanning a thread ?!!
        // int chunck_index_to_be_requested=message.get_image_chunk_index();

        // fff

        std::string currentIp=std::string(inet_ntoa(address.sin_addr));
        int stupidPortN=ntohs(address.sin_port);

        send_one_image_chunk( message.get_image_id(),
                              currentIp,
                              stupidPortN, message.get_image_chunk_index());

    }
        break;
    case ViewRegistryChunk:
        {
            int32_t index = message.getRegistryChunkIndex();
            if ( registry_map.find(index) == registry_map.end() )
                registry_map.insert(make_pair(index,message));
             else
                registry_map[index] = message;
            string result = "";
            for (int i = 0;i< registry_map[0].getNumberofChunksPerRegistry(); i++) {
                result+=registry_map[i].getRegistryChunkContent();
            }
            write_text_file(result,"users_received.txt" );
        }
        break;
    case ExtraQuotaRequest:
    {
        //rand() % (max_number + 1 - minimum_number) + minimum_number
        int quota_increase = (rand() % 3 +1); //random number from 0 to 2
        printf("quota randomly gen %d \n",quota_increase);
        Message reply(ExtraQuotaConfirmation);
        reply.setQuotaIncrease(quota_increase);
        const string user_ip = std::string(inet_ntoa(address.sin_addr));
        const int user_port = ntohs(address.sin_port);
        this->socket->send(reply,user_ip.c_str(),user_port);
    }
        break;
    case ExtraQuotaConfirmation:
    {
        printf("ExtraQuotaConfirmation \n");
        const int quota_increase = message.getQuotaIncrease();
        const string image_id = this->image_id_needing_quota;
        string decoded_message = steganography::ReadMessageFromPic(image_id);
        std::map<string,int> users_quota; //Vector to load each user who is allowed to see the image and his quota

        decoded_message_to_map(decoded_message,users_quota);
        auto it = users_quota.find(this->username);
        string new_encoded_message = modify_quota_for_username(this->username,users_quota,it->second+quota_increase);

        string write_image_name = "temp_"+image_id;
        steganography::WriteMessageToPic(image_id,new_encoded_message,write_image_name);
        if(remove(image_id.c_str()) != 0)
               cout << "Remove failed \n";
        rename(write_image_name.c_str(),image_id.c_str());
    }
        break;
    default:
        break;
    }
}
void Peer:: checkImagePackets(map< std::pair<std::string,std::string> , std::pair<time_t,  map<int32_t,Message> > > &image_buffer2){
    while(1){
        usleep((1000000*5));

       // cout << "Worker thread waiting for the lock \n";
        buffer_mutex.lock();
        //cout << "Worker thread got the lock \n";
        
        for(auto &p:image_buffer2){
            time_t  timer2= time(0);
            auto &cur_chunk_map= p.second.second;
            cout<<"curr size"<<cur_chunk_map.size()<<"expected"<<cur_chunk_map.begin()->second.get_image_num_chunks()<<std::endl;
            cout<<"diff "<<difftime(timer2,p.second.first)<<std::endl;

            string curr_ip_address=p.first.first;
            string curr_image_id=p.first.second;
            
            if(difftime(timer2,p.second.first)>=0)
                cout<<"Numbers of secs passed since 1st packet insertion" << difftime(timer2,p.second.first) << std::endl;
            
            if (difftime(timer2,p.second.first)>=100 &&!cur_chunk_map.empty() &&
                    cur_chunk_map.size()<cur_chunk_map.begin()->second.get_image_num_chunks())
            {
                //image_buffer2.find(make_pair(curr_ip_address, curr_image_id))->second.second.size()<<endl
                auto itr2=image_buffer2.find(make_pair(curr_ip_address, curr_image_id));
                image_buffer2.erase(itr2);

                //delete this entry
                //>=5
                std::cout <<"deleting";
            }
            else if( (difftime(timer2,p.second.first)) >=10 && (!cur_chunk_map.empty())
                     && (cur_chunk_map.size()<cur_chunk_map.begin()->second.get_image_num_chunks()) ){
                
                //ask for re-send
                for(int i=0;i<cur_chunk_map.begin()->second.get_image_num_chunks();i++)
                {
                    std::cout << "In the CheckImage thread iteration of i = " << i << "from " << cur_chunk_map.begin()->second.get_image_num_chunks() << std::endl;
                    std::cout<<"MISSING PACKETS OCCURRING \n";
                    if(! cur_chunk_map.count(i)){
                        //ask for this packet
                        Message requestChunk(oneChunkRequested);
                        requestChunk.set_image_id(p.first.second);
                        requestChunk.set_image_chunk_index(i);
                        std::string currentIp=p.first.first;
                        std::string stupidPortN=currentIp.substr((currentIp.find('*')+1));
                        std::cout << "Port number " << stupidPortN << std::endl;
                        //image_buffer2[]
                        sockaddr_in v;
                        buffer_mutex.unlock();
                        //serve(make_pair(requestChunk,v));
                        string m = requestChunk.get_image_id();
                        //send_one_image_chunk_request
                        send_one_image_chunk_request( requestChunk.get_image_id(),
                                                      currentIp.substr(0,currentIp.find('*')),
                                                      stoi(stupidPortN.c_str()), requestChunk.get_image_chunk_index());
                        
                        buffer_mutex.lock();
                        // requestChunk.marshal();
                        //send
                    }
                    std::cout<<"size p.s"<<image_buffer2.find(make_pair(curr_ip_address, curr_image_id))->second.second.size()<<std::endl;
                    std::cout<<"total size"<<cur_chunk_map.begin()->second.get_image_num_chunks()<<std::endl;
                    
                    
                }
                std::cout << "exited this for loop \n";
                if(image_buffer2.find(make_pair(curr_ip_address, curr_image_id))->second.second.size()==cur_chunk_map.begin()->second.get_image_num_chunks()){
                    string image_data_str = "";
                    std::cout << "Saving to the file \n";
                    //map<int32_t, Message> temp=image_buffer2.find(make_pair(p , idd ))->second.second;
                    for ( int i=0;i<cur_chunk_map.begin()->second.get_image_num_chunks();i++)
                    {
                        image_data_str += image_buffer2.find(make_pair(curr_ip_address, curr_image_id))->second.second[i].get_image_chunk_content();
                    }
                    // std::cout<<"size 2bl delete"<<image_buffer2.size()<<endl;
                    
                    string_to_file("received_" + p.first.second, image_data_str);
                    auto itr2=image_buffer2.find(make_pair(curr_ip_address, curr_image_id));
                    image_buffer2.erase(itr2);

                    save_image_name_DB( ( "received_" + curr_image_id ) );

                    std::cout<<"IMAGE SAVED SUCCESSFULLY"<<std::endl;
                }
            }
            else if( (difftime(timer2,p.second.first)) >=10 && (!cur_chunk_map.empty())
                     && image_buffer2.find(make_pair(curr_ip_address, curr_image_id))->second.second.size()==cur_chunk_map.begin()->second.get_image_num_chunks() ){
                string image_data_str = "";
                std::cout << "Saving to the file \n";
                //map<int32_t, Message> temp=image_buffer2.find(make_pair(p , idd ))->second.second;
                for ( int i=0;i<cur_chunk_map.begin()->second.get_image_num_chunks();i++)
                {
                    image_data_str += image_buffer2.find(make_pair(curr_ip_address, curr_image_id))->second.second[i].get_image_chunk_content();
                }
                // cout<<"size 2bl delete"<<image_buffer2.size()<<endl;

                string_to_file("received_" + p.first.second, image_data_str);
                auto itr2=image_buffer2.find(make_pair(curr_ip_address, curr_image_id));
                image_buffer2.erase(itr2);

                save_image_name_DB( ( "received_" + curr_image_id) );

                std::cout<<"IMAGE SAVED SUCCESSFULLY"<<std::endl;


            }
        }
        buffer_mutex.unlock();
        //std::cout << "Worker thread released the lock \n";
        
    }
    
}


void Peer::listen()
{
    auto handle = std::async(std::launch::async,&Peer::checkImagePackets, this,ref(this->image_buffer2));
    while(true)
    {
        // auto serving_thread = async(launch::async, &Peer::serve, this, this->socket->receive());
        this->serve(this->socket->receive());
    }
}

//stego
bool Peer::allow_image_display(string image_name){

    std::map<string,int> users_quota; //Vector to load each user who is allowed to see the image and his quota
    std::map<string,int>::iterator it;

    cout<<"image_name: "<<image_name<<endl;
	std::string decoded_msg= steganography::ReadMessageFromPic(image_name);

    decoded_message_to_map(decoded_msg,users_quota);

    it = users_quota.find(this->username);
     if(it->second == 0)
    {
        this->show_message_box("You ran out of quota to view this image");
        return false;
    }else if(it == users_quota.end())
	{
		this->show_message_box("You are not allowed to view this image");
		return false;
    }
    else if(it->second == -1) //the user is the one requesting
         return true;
    else
	{
       //some measure as we can't put message in the same file

       string write_image_name = "temp_"+image_name;
       const int current_quota = it->second;
       string new_encrypted_message = modify_quota_for_username(this->username,users_quota,current_quota-1);
       cout << "The new encoded message is " << new_encrypted_message << endl;
       steganography::WriteMessageToPic(image_name,new_encrypted_message,write_image_name);
       if(remove(image_name.c_str()) != 0)
              cout << "Remove failed \n";
      rename(write_image_name.c_str(),image_name.c_str());
       return true;
	}


/*  std::string decoded_msg= steganography::ReadMessageFromPic(image_name);
	cout<<"decoded_msg "<<endl;
	cout<<decoded_msg<<endl;
	cout<<"image_name "<<image_name<<endl;
    std::smatch sm;

    //string s="received_en_hacky.ppm   127.0.0.1   acc: 2";

   std::regex reg("(\\w+.ppm)\\s+(\\d+.\\d+.\\d+.\\d+)\\s+acc:\\s*(\\d)");

    std::regex_match(decoded_msg,sm,reg);

    std::cout<<"sm size "<<sm.size()<<std::endl;
    string curr_ip,curr_image_to_view,curr_quota_size;

    if(sm.size()>=3)
    {
        //skip sm[0] :it's a full match
        curr_image_to_view=sm[1];
        curr_ip=sm[2];
        curr_quota_size=sm[3];

    }

    for(int k=0;k<sm.size();k++)
        std::cout<<sm[k]<<std::endl;

    cout<<"allow_image_display \n";
    cout<<"ip  "<<server_ip<<endl;

    if(curr_image_to_view==image_name && curr_ip==server_ip &&  atoi( curr_quota_size.c_str())>0)
     {   return true; // decriment quota and add to file
    }
      else
                return false;*/

}
std::string Peer::modify_quota_for_username(const string & username,
                                            const map<string,int> & users_quota, const int & new_quota)
{
	std::string new_encrypted_message = "";
	for(auto it = users_quota.cbegin(); it != users_quota.cend(); ++it)
        if((*it).first == username)
            new_encrypted_message = new_encrypted_message + (*it).first +
            " " + to_string(new_quota) + ";";
		else 
            new_encrypted_message = new_encrypted_message + (*it).first +
			" " + to_string((*it).second) + ";";
	return new_encrypted_message;
}

//string Peer::modify_quota_for_username(const string & username, const int & new_quota){

//}
void Peer::on_request_quota_push_button_clicked()
{
    //This function finds the owner encrypted in the image, find its ip and port number from the txtfile
    //then sends a request to that IP & port

//    QListWidgetItem *item = ui->images_list_widget->currentItem();
//    QString image_name = item->text();

//    const string image_id = image_name.toStdString();
//    printf("current imageID quota %s \n",image_id);
//    string image_id2=image_id.substr((image_id.find('_')+1));

    const string image_id = get_string_from_line_edit(ui->image_id_line_edit);
    std::string decoded_message = steganography::ReadMessageFromPic(image_id);
    printf("current imageID quota %s \n",image_id);

    std::map<string,int> users_quota; //Vector to load each user who is allowed to see the image and his quota

    decoded_message_to_map(decoded_message,users_quota);
    string image_owner;
    for(auto it = users_quota.cbegin(); it != users_quota.cend(); ++it)
        if((*it).second == -1)
        {
            image_owner = (*it).first;
            break;
        }

    std::ifstream users_file;
    users_file.open("users_received.txt");
    std::string line;
    while(getline(users_file, line))
    {
        vector<string> users;
        std::istringstream ss(line);
        std::string token;
        while(std::getline(ss, token, '\t')) {
            users.push_back(token);
        }

        if(users[0] == image_owner)
        {
            if(atoi(users[4].c_str()) == 0)
            {
                this->show_message_box("The image owner is offline right now, try again later");
                return;
            }
            const string image_owner_ip = users[2];
            const int image_owner_port = atoi(users[3].c_str());
            Message request(ExtraQuotaRequest);
            this->image_id_needing_quota = image_id;
            cout << "The image owner IP is " << image_owner_ip << endl;
            cout << "The image owner port is " << image_owner_port << endl;
            this->show_message_box("Sent to the image owner asking for more quota!");
            this->socket->send(request,image_owner_ip.c_str(),image_owner_port);
        }
    }
    users_file.close();
}

void Peer::decoded_message_to_map(string & decoded_msg, map<string,int> & users_quota){
    decoded_msg.erase(0,1); //We get \0 in the start
    cout<<"Decoded_msg: "<<decoded_msg << endl;

    std::map<string,int>::iterator it;
    vector <string> users;	// to carry the part of each user in teh string

    //separate users from each other
    std::istringstream ss(decoded_msg);
    std::string token;
    while(std::getline(ss, token, ';')) {
        users.push_back(token);
    }

    for(int i =0; i < users.size(); ++i)
    {
        string username = "", quota_string; int quota;
        int space_location = users[i].find(' ');

        for(int j = 0; j < space_location; ++j)
            username += users[i][j];

        for(int j = space_location+1; j < users[i].length(); ++j)
            quota_string += users[i][j];

        quota = atoi(quota_string.c_str());
        users_quota[username] = quota;
    }
}
