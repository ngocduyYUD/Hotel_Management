#include <stdio.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <libpq-fe.h>

typedef struct {
    gchar *room;
    gchar *state;
    gchar *customer;
}Hotel;


static Hotel info[100];
static int cur=0;
static int curTemp=0;


// cua so login
GtkBuilder *builder;
GtkWidget *loginWindow;
GtkWidget *notiLabel1;
GtkWidget *loginfixed;
GtkWidget *usernameEntry;
GtkWidget *passEntry;
GtkWidget *loginBtn;

//main window
GtkWidget *mainwindow;
GtkWidget *mainlabel;
GtkWidget *resultlabel;
GtkWidget *mainfixed;
GtkWidget *roomsearch;
GtkWidget *logoutBtn;
GtkWidget *checkinBtn;
GtkWidget *checkoutBtn;
GtkWidget *refreshBtn;
GtkWidget *searchentryBtn;
GtkWidget *servicebillBtn;
GtkWidget *roombillBtn;
GtkWidget *serveBtn;
GtkWidget *useserviceBtn;
GtkWidget *emptyBtn;
GtkWidget *searchBtn;
GtkTreeSelection *roomselection;
GtkTreeView *roomview;

//service window
GtkWidget *servicewindow;
GtkTreeSelection *serviceselection;
GtkTreeView *serviceview;
GtkTreeIter serveiter;

//roominfowindow
GtkWidget *roominfowindow;
GtkWidget *roominfolabel;
GtkWidget *roomtypelabel;
GtkWidget *freeservicelabel;
GtkWidget *daypricelabel;
GtkWidget *hourlabel;

//useservicewindow
GtkWidget *useservicewindow;
GtkWidget *custuseserviceEntry;
GtkWidget *useserviceEntry;
GtkWidget *servicenumEntry;
GtkWidget *serviceusageBtn;
GtkWidget *useservicelabel;

//check in window (Tan)
GtkWidget *checkinwindow;
GtkWidget *checkinNameEntry;
GtkWidget *checkinPhoneEntry;
GtkWidget *checkinCMNDEntry;
GtkWidget *checkinRoomEntry;
GtkWidget *chechkinAddBtn;
GtkWidget *checkinLabel;
GtkWidget *reportLabel;
//check out window (Tan)
GtkWidget *checkoutwindow;
GtkWidget *checkoutButton;
GtkWidget *checkoutLabel;
GtkWidget *checkoutRoomEntry;

//service bill window (Tan)
GtkWidget *servicebillwindow;
GtkWidget *servicebillEntry;
GtkWidget *searchbillButton;
GtkWidget *billButton;//Nut xuat hoa don
GtkWidget *namecustomerinbillLabel;//Hien thi ten Khach hang
GtkWidget *serviceinbillLabel;//service in bill
GtkWidget *servicepriceinbillLabel;//service price in bill
GtkWidget *totalpriceLabel;//Hien thi tong tien

//Duy 
//emptyroomwindow
GtkWidget *emptyroomwindow;
GtkTreeSelection *emptyroomselection;
GtkTreeView *emptyroomview;
GtkTreeIter roomiter;
//roomsearchwindow
GtkWidget *roomsearchwindow;
GtkWidget *roomsearchEntry;
GtkWidget *roomsearchOkBtn;
GtkWidget *notiLabel2;
GtkWidget *roomsearchFixed;
//roomsearchwindow2
GtkWidget *roomsearchwindow2;
GtkTreeSelection *roomsearchwindow2selection;
GtkTreeView *roomsearchwindow2view;
GtkTreeIter roomsearchiter;
// loai phong khach san
GtkWidget *lpks;
GtkTreeSelection *lpksselection;
GtkTreeView *lpksview;
GtkTreeIter lpksIter;

//Duy
//roombillbtnwindow1
GtkWidget *roombillWindow1Fixed;
GtkWidget *CusIDEntry;
GtkWidget *CusIDokBtn;
GtkWidget *notiRoombill;
GtkWidget *roombillWindow1;
GtkWidget *roomnameEntry;
GtkWidget *arrivalEntry;
GtkWidget *departureEntry;
//roombillWindow2
GtkWidget *roombillWindow2;
GtkWidget *roombillnamelabel;
GtkWidget *cmndlabel;
GtkWidget *roombillroomlabel;
GtkWidget *arrivallabel;
GtkWidget *departurelabel;
GtkWidget *roombillmoneylabel;




//Tan
void on_checkinBtn_clicked();//Ham hien thi cua so checkin
void on_checkinadd_clicked();//an "Them" trong Checkin

void on_checkoutBtn_clicked();//Ham hien thi cua so check out 
void on_checkoutButton_clicked(); //Khi an nut "Xac nhan" trong Checkout

void on_servicebill_clicked();//Khi an vao "Xuat hoa don", Hoa don dich vu
void on_searchbillButton_clicked(); // Khi an vao "TIm" ma KH trong Hoa don dich vu
void on_billButton_clicked();//Khi an vao "Thanh toan" trong Hoa don dich vu

//Duy
void on_emptyBtn_clicked();         // bam nut phong trong, in ra danh sach phong trong luon
void on_emptyroomwindow_destroy();  
void on_searchBtn_clicked();        //bam nut tim phong 
void on_roomsearchOkBtn_clicked();  //bam nut ok trong cua so tim phong
void on_roomsearchwindow2_destroy();  //man hinh cac phong trong theo ma phong
void on_roombillWindow2_destroy();
void on_roomsearchwindow_destroy();
void on_lpks_destroy();

//Dung
void on_serveBtn_clicked();
void on_servicewindow_destroy();
void on_roomview_row_activated();
void on_useservicewindow_destroyed();
void on_serviceusageBtn_clicked();


GtkTreeIter curIter;
GtkListStore *store;

GtkTreeModel *create_model(Hotel dataParam[]){
    gint i=0;
    store = gtk_list_store_new(3, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
    for(i = 0; i < cur; ++i){
        gtk_list_store_append(store, &curIter);
        gtk_list_store_set(store, &curIter, 0, dataParam[i].room, 
                            1, dataParam[i].customer, 
                            2, dataParam[i].state, -1);
    }
    return GTK_TREE_MODEL(store);
}

GtkTreeViewColumn *column1, *column2, *column3;
GtkCellRenderer *renderer,*renderer1;

void add_room_columns(GtkTreeView *treeview){

    GtkTreeModel *model = gtk_tree_view_get_model(treeview);
    renderer = gtk_cell_renderer_text_new();
    column1 = gtk_tree_view_column_new_with_attributes("Phong", renderer, "text", 0, NULL);
    column2 = gtk_tree_view_column_new_with_attributes("Khach hang", renderer, "text", 1, NULL);
    column3 = gtk_tree_view_column_new_with_attributes("Trang thai", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(treeview, column1);
    gtk_tree_view_append_column(treeview, column2);
    gtk_tree_view_append_column(treeview, column3);
}


void do_exit(PGconn *conn) {
    
    fprintf(stderr, "%s\n", PQerrorMessage(conn));    
    PQfinish(conn);    
    
    exit(1);
}

int main(int argc, char *argv[]){
    if (argc < 2) {
        g_print("Usage: ./hotel ./hotel.glade\n");
        return 1;
    }

    gtk_init(&argc, &argv);
    builder = gtk_builder_new_from_file(argv[1]);
    loginWindow = GTK_WIDGET(gtk_builder_get_object(builder, "loginWindow"));
    g_signal_connect(loginWindow, "destroy", G_CALLBACK(gtk_main_quit), "NULL");
    gtk_builder_connect_signals(builder, NULL);

    loginfixed = GTK_WIDGET(gtk_builder_get_object(builder, "loginfixed"));
    notiLabel1 = GTK_WIDGET(gtk_builder_get_object(builder, "notiLabel1"));
    passEntry = GTK_WIDGET(gtk_builder_get_object(builder, "passEntry"));
    gtk_entry_set_visibility (GTK_ENTRY(passEntry), FALSE);
    usernameEntry = GTK_WIDGET(gtk_builder_get_object(builder, "usernameEntry"));
    loginBtn = GTK_WIDGET(gtk_builder_get_object(builder, "loginBtn"));
    
   
    gtk_widget_set_sensitive(loginBtn, FALSE);

    gtk_widget_show(loginWindow);
    gtk_main();
    return 0;
}

void on_usernameEntry_changed(GtkEntry *e){
    gtk_widget_set_sensitive(loginBtn, TRUE);
}

void on_passEntry_changed(GtkEntry *e){
    gtk_widget_set_sensitive(loginBtn, TRUE);
}

void on_logoutBtn_clicked()
{
    gtk_widget_destroy(mainwindow);
    gtk_widget_set_sensitive(loginWindow, TRUE);
}

void on_loginBtn_clicked()
{
    char username[50];
    char pass[50];
    sprintf(username, "%s", gtk_entry_get_text(GTK_ENTRY(usernameEntry)));
    sprintf(pass, "%s", gtk_entry_get_text(GTK_ENTRY(passEntry)));

    const char *conninfo;
    PGconn *conn; 
    conninfo = "host=localhost user=postgres password=root dbname=hotel";
    conn = PQconnectdb(conninfo);
    if(PQstatus(conn) != CONNECTION_OK)
    {
        printf("Connection to database failed %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }

    PGresult *res = PQexec(conn, "SELECT ma_nhan_vien, mat_khau FROM nhan_vien"); 
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("No data retrieved\n");        
        PQclear(res);
        do_exit(conn);
    }    
    
    int rows = PQntuples(res);
    int count = 0;
    
    for(int i=0; i<rows; i++) {
        printf("%s%s%s%s\n", PQgetvalue(res, i, 0), username, PQgetvalue(res, i, 1), pass);
        if(strcmp(username, PQgetvalue(res, i, 0))==0 && strcmp(pass, PQgetvalue(res, i, 1))==0)
        {
            count++;
            printf("Found info\n");
            break;
        }
    }
    if(count<=0) gtk_label_set_text(GTK_LABEL(notiLabel1), (const gchar*)"Sai ten tai khoan hoac mat khau ");
    else
    {
        gtk_label_set_text(GTK_LABEL(notiLabel1), (const gchar*)"Log in successfully");  
        builder = gtk_builder_new_from_file("hotel.glade");
        mainwindow = GTK_WIDGET(gtk_builder_get_object(builder, "mainwindow"));
        g_signal_connect(mainwindow, "destroy", G_CALLBACK(on_logoutBtn_clicked), "NULL");
        gtk_builder_connect_signals(builder, NULL);
        mainfixed = GTK_WIDGET(gtk_builder_get_object(builder, "mainfixed"));
        mainlabel = GTK_WIDGET(gtk_builder_get_object(builder, "mainlabel"));
        resultlabel = GTK_WIDGET(gtk_builder_get_object(builder, "resultlabel"));
        roomsearch = GTK_WIDGET(gtk_builder_get_object(builder, "roomsearch"));
        logoutBtn = GTK_WIDGET(gtk_builder_get_object(builder, "logoutBtn"));
        g_signal_connect(logoutBtn, "clicked", G_CALLBACK(on_logoutBtn_clicked), "NULL");
        checkinBtn = GTK_WIDGET(gtk_builder_get_object(builder, "checkinBtn"));
        checkoutBtn = GTK_WIDGET(gtk_builder_get_object(builder, "checkoutBtn"));
        servicebillBtn = GTK_WIDGET(gtk_builder_get_object(builder, "servicebillBtn"));
        roombillBtn = GTK_WIDGET(gtk_builder_get_object(builder, "roombillBtn"));
        serveBtn = GTK_WIDGET(gtk_builder_get_object(builder, "serveBtn"));
        useserviceBtn = GTK_WIDGET(gtk_builder_get_object(builder, "useserviceBtn"));
        emptyBtn = GTK_WIDGET(gtk_builder_get_object(builder, "emptyBtn"));
        searchBtn = GTK_WIDGET(gtk_builder_get_object(builder, "searchBtn"));
        searchentryBtn = GTK_WIDGET(gtk_builder_get_object(builder, "searchentryBtn"));
        const char *holder = "Nhap ten phong";
        gtk_entry_set_placeholder_text (GTK_ENTRY(roomsearch), holder);
        refreshBtn = GTK_WIDGET(gtk_builder_get_object(builder, "refreshBtn"));
        roomview = GTK_TREE_VIEW(gtk_builder_get_object(builder, "roomview"));
        roomselection = GTK_TREE_SELECTION(gtk_builder_get_object(builder, "roomselection"));

        res = PQexec(conn, "SELECT phong.ten_phong, khach_hang.ho_ten, phong_kem_phieu_thue.ngay_di FROM phong LEFT JOIN phong_kem_phieu_thue using(ma_phong) LEFT JOIN phieu_thue using(ma_phieu_thue) LEFT JOIN khach_hang using(ma_khach_hang)"); 
        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("No room and customer data retrieved\n");        
        PQclear(res);
        //do_exit(conn);
        }
        rows = PQntuples(res);
    
        int count = 0;
        for(int i=0; i<rows; i++) 
        {
            info[count].room=(char*)malloc(strlen(PQgetvalue(res, i, 0))*sizeof(char));
            strcpy(info[count].room, PQgetvalue(res, i, 0));
            info[count].customer=(char*)malloc(strlen(PQgetvalue(res, i, 1))*sizeof(char));
            if(i>0&&strcmp(PQgetvalue(res, i, 0), PQgetvalue(res, i-1, 0))==0&&strcmp(info[count-1].state, "Co nguoi") ==0) continue;
            
            if(i>0&&strcmp(PQgetvalue(res, i, 0), PQgetvalue(res, i-1, 0))==0&&strcmp(info[count-1].state, "Co nguoi") !=0) count--;
            

            if(strlen(PQgetvalue(res, i, 1)) == 0 || strlen(PQgetvalue(res, i, 2))>0) 
            {
                //printf("Phong khong co nguoi\n");
                info[count].customer = "";
                info[count].state = "Khong co nguoi";
                count++;
            }
            else 
            {
                strcpy(info[count].customer, PQgetvalue(res, i, 1));
                info[count].state = "Co nguoi";
                //printf("Co nguoi\n");
                count++;
            }
            cur++;
        }  

        gtk_tree_view_set_model(GTK_TREE_VIEW(roomview), GTK_TREE_MODEL(create_model(info)));  
        add_room_columns(GTK_TREE_VIEW(roomview));  
        gtk_widget_set_sensitive(loginWindow,FALSE); 
        
        //set up thanh tim kiem
        GtkEntryCompletion *completion = gtk_entry_completion_new();
    
        gtk_entry_completion_set_model(completion, gtk_tree_view_get_model(roomview));
        gtk_entry_completion_set_text_column (completion, 0);
        gtk_entry_set_completion(GTK_ENTRY(roomsearch), completion);
        gtk_widget_show(mainwindow);
    } 
    PQclear(res);
    PQfinish(conn);
}

void on_useserviceBtn_clicked()
{
    gtk_widget_set_sensitive(useserviceBtn, FALSE);
    builder = gtk_builder_new_from_file("hotel.glade");
    useservicewindow = GTK_WIDGET(gtk_builder_get_object(builder, "useservicewindow"));
    g_signal_connect(useservicewindow, "destroy", G_CALLBACK(on_useservicewindow_destroyed), "NULL");
    custuseserviceEntry = GTK_WIDGET(gtk_builder_get_object(builder, "custuseserviceEntry"));
    useserviceEntry = GTK_WIDGET(gtk_builder_get_object(builder, "useserviceEntry"));
    servicenumEntry = GTK_WIDGET(gtk_builder_get_object(builder, "servicenumEntry"));
    serviceusageBtn = GTK_WIDGET(gtk_builder_get_object(builder, "serviceusageBtn"));
    g_signal_connect(serviceusageBtn, "clicked", G_CALLBACK(on_serviceusageBtn_clicked), "NULL");
    useservicelabel = GTK_WIDGET(gtk_builder_get_object(builder, "useservicelabel"));
    gtk_widget_show(useservicewindow);

}

void on_useservicewindow_destroyed()
{
    gtk_widget_destroy(useservicewindow);
    gtk_widget_set_sensitive(useserviceBtn, TRUE);
}

void on_serviceusageBtn_clicked()
{
    gtk_label_set_text(GTK_LABEL(useservicelabel), (const gchar*)"");
    printf("received\n");
    char custID[20];
    char service[20];
    char tmp[10];
    sprintf(custID, "%s", gtk_entry_get_text(GTK_ENTRY(custuseserviceEntry)));
    sprintf(service, "%s", gtk_entry_get_text(GTK_ENTRY(useserviceEntry)));
    sprintf(tmp, "%s", gtk_entry_get_text(GTK_ENTRY(servicenumEntry)));
    //printf("%s\n", custID);
    //printf("%s\n", service);
    int num = atoi(tmp);
    const char *conninfo;
    PGconn *conn; 
    conninfo = "host=localhost user=postgres password=root dbname=hotel";
    conn = PQconnectdb(conninfo);
    if(PQstatus(conn) != CONNECTION_OK)
    {
        printf("Connection to database failed %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }
    char command[255] = "SELECT khach_hang.ma_khach_hang FROM khach_hang join phieu_thue using(ma_khach_hang) join phong_kem_phieu_thue using (ma_phieu_thue) where ma_khach_hang = \'";
    strcat(command, custID);
    strcat(command, "\';");
    printf("Connected\n");

    PGresult *res = PQexec(conn, command);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("No customer data retrieved\n");        
        PQclear(res);
        //do_exit(conn);
    }
    printf("Done checking step 1\n");
    int rows = PQntuples(res);
    int count = 0;
    if(rows==0) 
    {
        gtk_label_set_text(GTK_LABEL(useservicelabel), (const gchar*)"Sai ma khach hang");
        return;
    }
    
    char command2[255] = "SELECT ma_dich_vu, ten_dich_vu FROM dich_vu where ten_dich_vu = \'";
    strcat(command2, service);
    strcat(command2, "\';");
    res = PQexec(conn, command2);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("No service data retrieved\n");        
        PQclear(res);
    }
    rows = PQntuples(res);
    if(rows==0) 
    {
        gtk_label_set_text(GTK_LABEL(useservicelabel), (const gchar*)"Khong co loai hinh dich vu nay");
        return;
    }
    printf("Done checking step 2\n");
    strcpy(service, PQgetvalue(res, 0, 0));
    char maHD[10] = "HV";
    char command3[150] = "SELECT ma_khach_hang, ma_hoa_don FROM lich_su_su_dungdv";
    res = PQexec(conn, command3);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("No service history data retrieved\n");        
        PQclear(res);
    }
    rows = PQntuples(res);
    for(int i=0; i<rows; i++)
    {
        if(strcmp(PQgetvalue(res, i, 0), custID)==0)
        {
            strcpy(maHD, PQgetvalue(res, i, 1));
            count ++;
            break;
        }
    }
    if(count==0)
    {
        char command3_25[255] = "SELECT distinct ma_hoa_don FROM lich_su_su_dungdv";
        res = PQexec(conn, command3_25);
        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            printf("No service history data retrieved\n");        
            PQclear(res);
        }
        char num[5];
        rows = PQntuples(res);
        sprintf(num, "%d", rows+1);
        for(int i=0; i<4-strlen(num); i++)
        {
            strcat(maHD, "0");
        }
        strcat(maHD, num);
        int n=rows+1;
        for(int i=0; i<rows; i++)
        {
            if(strcmp(maHD, PQgetvalue(res, i, 0))==0)
            {
                strcpy(maHD, "HV");
                n++;
            }
        }
        sprintf(num, "%d", n);
        for(int i=0; i<4-strlen(num); i++)
        {
            strcat(maHD, "0");
        }
        strcat(maHD, num);

        char command3_5[255] = "insert into hoa_don(ma_hoa_don, ma_khach_hang, ngay_lap, tong_tien) values (\'";
        strcat(command3_5, maHD);
        strcat(command3_5,"\',\'");
        strcat(command3_5, custID);
        strcat(command3_5,"\', CURRENT_DATE, \'0\')");
        PQexec(conn, command3_5);

    }
    char command4[255] = "insert into lich_su_su_dungdv(ma_dich_vu, ma_hoa_don, ma_khach_hang) values (\'";
    strcat(command4, service);
    strcat(command4,"\',\'");
    strcat(command4, maHD);
    strcat(command4,"\',\'");
    strcat(command4, custID);
    strcat(command4,"\')");
    
    for(int i=0; i<num; i++) PQexec(conn, command4);

    gtk_label_set_text(GTK_LABEL(useservicelabel), (const gchar*)"Them thanh cong");
}


void on_searchentryBtn_clicked()
{
    char search[50];
    sprintf(search, "%s", gtk_entry_get_text(GTK_ENTRY(roomsearch)));
    const char *conninfo;
    PGconn *conn; 
    conninfo = "host=localhost user=postgres password=root dbname=hotel";
    conn = PQconnectdb(conninfo);
    if(PQstatus(conn) != CONNECTION_OK)
    {
        printf("Connection to database failed %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }
    char *command;
    command = "SELECT ten_phong FROM phong";

    PGresult *res = PQexec(conn, command);
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("No room and customer data retrieved\n");        
        PQclear(res);
        //do_exit(conn);
    }
    int rows = PQntuples(res);
    int count = 0;
    
    for(int i=0; i<rows; i++) {
        printf("%s%s\n", PQgetvalue(res, i, 0), search);
        if(strcmp(search, PQgetvalue(res, i, 0))==0)
        {
            count++;
            printf("Found info\n");
            break;
        }
    }
    if(count<=0) {
        gtk_label_set_text(GTK_LABEL(resultlabel), (const gchar*)"Khong tim thay phong");
        return;
    }

    //giong voi on_row_activated

    command = "SELECT phong.ten_phong, loai_phong.loai_phong, loai_phong.thue_theo_ngay, loai_phong.thue_theo_gio, dich_vu.ten_dich_vu FROM phong LEFT JOIN loai_phong using(ma_loai_phong) LEFT JOIN dich_vu_free using(ma_loai_phong) LEFT JOIN dich_vu using(ma_dich_vu)";
    res = PQexec(conn, command); 
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("No room name retrieved\n");        
        PQclear(res);
        do_exit(conn);
    }    

    rows = PQntuples(res);
    
    builder = gtk_builder_new_from_file("hotel.glade");
    roominfowindow = GTK_WIDGET(gtk_builder_get_object(builder, "roominfowindow"));
    roominfolabel = GTK_WIDGET(gtk_builder_get_object(builder, "roominfolabel"));
    roomtypelabel = GTK_WIDGET(gtk_builder_get_object(builder, "roomtypelabel"));
    daypricelabel = GTK_WIDGET(gtk_builder_get_object(builder, "daypricelabel"));
    hourlabel = GTK_WIDGET(gtk_builder_get_object(builder, "hourlabel"));
    freeservicelabel = GTK_WIDGET(gtk_builder_get_object(builder, "freeservicelabel"));

    //gtk_label_set_text(GTK_LABEL(notiLabel2), (const char*)tmp);

    gtk_widget_show(roominfowindow);

    char tmp[255] = "";
    for(int i=0; i<rows; i++) {
        if(strcmp(search, PQgetvalue(res, i, 0))==0)
        {
            gtk_label_set_text(GTK_LABEL(roominfolabel), (const char*)search);
            gtk_label_set_text(GTK_LABEL(roomtypelabel), (const char*)PQgetvalue(res, i, 1));
            gtk_label_set_text(GTK_LABEL(daypricelabel), (const char*)PQgetvalue(res, i, 2));
            gtk_label_set_text(GTK_LABEL(hourlabel), (const char*)PQgetvalue(res, i, 3));
            strcat(tmp, PQgetvalue(res, i, 4));

            if(i==rows-1||strcmp(search, PQgetvalue(res, i+1, 0))!=0) break;
            strcat(tmp, ", ");
        }
    }
    gtk_label_set_text(GTK_LABEL(freeservicelabel), (const char*)tmp);

    PQclear(res);
    PQfinish(conn);


}

//refresh
void on_refreshBtn_clicked()
{
    const char *conninfo;
    PGconn *conn; 
    conninfo = "host=localhost user=postgres password=root dbname=hotel";
    conn = PQconnectdb(conninfo);
    if(PQstatus(conn) != CONNECTION_OK)
    {
        printf("Connection to database failed %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }

    PGresult *res = PQexec(conn, "SELECT phong.ten_phong, khach_hang.ho_ten, phong_kem_phieu_thue.ngay_di FROM phong LEFT JOIN phong_kem_phieu_thue using(ma_phong) LEFT JOIN phieu_thue using(ma_phieu_thue) LEFT JOIN khach_hang using(ma_khach_hang)"); 
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("No room and customer data retrieved\n");        
        PQclear(res);
        //do_exit(conn);
    }
    int rows = PQntuples(res);
    int count = 0;
    for(int i=0; i<rows; i++) 
    {
        info[count].room=(char*)malloc(strlen(PQgetvalue(res, i, 0))*sizeof(char));
        strcpy(info[count].room, PQgetvalue(res, i, 0));
        info[count].customer=(char*)malloc(strlen(PQgetvalue(res, i, 1))*sizeof(char));

        if(i>0&&strcmp(PQgetvalue(res, i, 0), PQgetvalue(res, i-1, 0))==0&&strcmp(info[count-1].state, "Co nguoi") ==0) continue;
            
        if(i>0&&strcmp(PQgetvalue(res, i, 0), PQgetvalue(res, i-1, 0))==0&&strcmp(info[count-1].state, "Co nguoi") !=0) count--;
            
        if(strlen(PQgetvalue(res, i, 1)) == 0 || strlen(PQgetvalue(res, i, 2))>0) 
        {
            //printf("Phong khong co nguoi\n");
            info[count].customer = "";
            info[count].state = "Khong co nguoi";
            count++;
        }
            else 
            {
                strcpy(info[count].customer, PQgetvalue(res, i, 1));
                info[count].state = "Co nguoi";
                //printf("Co nguoi\n");
                count++;
            }
        cur++;
    }

    gtk_tree_view_set_model(GTK_TREE_VIEW(roomview), GTK_TREE_MODEL(create_model(info)));  

    PQclear(res);
    PQfinish(conn);

}

//danh sach dich vu
void on_serveBtn_clicked()
{
    gtk_widget_set_sensitive(serveBtn, FALSE);
    builder = gtk_builder_new_from_file("hotel.glade");
    servicewindow = GTK_WIDGET(gtk_builder_get_object(builder, "servicewindow"));
    g_signal_connect(servicewindow, "destroy", G_CALLBACK(on_servicewindow_destroy), "NULL");
    serviceview = GTK_TREE_VIEW(gtk_builder_get_object(builder, "serviceview"));
    serviceselection = GTK_TREE_SELECTION(gtk_builder_get_object(builder, "serviceselection"));

    GtkListStore *service = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);

    const char *conninfo;
    PGconn *conn; 
    conninfo = "host=localhost user=postgres password=root dbname=hotel";
    conn = PQconnectdb(conninfo);
    if(PQstatus(conn) != CONNECTION_OK)
    {
        printf("Connection to database failed %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }

    PGresult *res = PQexec(conn, "SELECT ten_dich_vu, don_gia FROM dich_vu"); 
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("No service data retrieved\n");        
        PQclear(res);
    }    
    
    int rows = PQntuples(res);
    
    for(int i=0; i<rows; i++) {
        gtk_list_store_append(service, &serveiter);
        gtk_list_store_set(service, &serveiter, 0, PQgetvalue(res, i, 0), 1, PQgetvalue(res, i, 1), -1);
    }

    gtk_tree_view_set_model(GTK_TREE_VIEW(serviceview), GTK_TREE_MODEL(service));   
    GtkTreeViewColumn *col1, *col2;
    GtkCellRenderer *renderer;
    GtkTreeModel *model = gtk_tree_view_get_model(serviceview);
    renderer = gtk_cell_renderer_text_new();
    col1 = gtk_tree_view_column_new_with_attributes("Ten dich vu", renderer, "text", 0, NULL);
    col2 = gtk_tree_view_column_new_with_attributes("Don gia", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(serviceview, col1);
    gtk_tree_view_append_column(serviceview, col2);

    gtk_widget_show(servicewindow);
    PQclear(res);
    PQfinish(conn);
}

void on_servicewindow_destroy(){
    gtk_widget_destroy(servicewindow);
    gtk_widget_set_sensitive(serveBtn, TRUE);
}

//roominfo

void on_roomview_row_activated(GtkTreeView *view, GtkTreePath *path, GtkTreeViewColumn *col, gpointer user_data)
{
    gtk_widget_destroy(roominfowindow);
    GtkTreeModel *model;
    GtkTreeIter   iter;

    model = gtk_tree_view_get_model(view);

    if (!gtk_tree_model_get_iter(model, &iter, path))
        return;

    const gchar *name;
    gtk_tree_model_get(model, &iter, 0, &name, -1);

    //printf("%s\n", name);
    const char *conninfo;
    PGconn *conn; 
    conninfo = "host=localhost user=postgres password=root dbname=hotel";
    conn = PQconnectdb(conninfo);
    if(PQstatus(conn) != CONNECTION_OK)
    {
        printf("Connection to database failed %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }

    char *command;
    command = "SELECT phong.ten_phong, loai_phong.loai_phong, loai_phong.thue_theo_ngay, loai_phong.thue_theo_gio, dich_vu.ten_dich_vu FROM phong LEFT JOIN loai_phong using(ma_loai_phong) LEFT JOIN dich_vu_free using(ma_loai_phong) LEFT JOIN dich_vu using(ma_dich_vu)";
   
    PGresult *res = PQexec(conn, command); 
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("No room name retrieved\n");        
        PQclear(res);
        do_exit(conn);
    }    

    int rows = PQntuples(res);
    
    builder = gtk_builder_new_from_file("hotel.glade");
    roominfowindow = GTK_WIDGET(gtk_builder_get_object(builder, "roominfowindow"));
    roominfolabel = GTK_WIDGET(gtk_builder_get_object(builder, "roominfolabel"));
    roomtypelabel = GTK_WIDGET(gtk_builder_get_object(builder, "roomtypelabel"));
    daypricelabel = GTK_WIDGET(gtk_builder_get_object(builder, "daypricelabel"));
    hourlabel = GTK_WIDGET(gtk_builder_get_object(builder, "hourlabel"));
    freeservicelabel = GTK_WIDGET(gtk_builder_get_object(builder, "freeservicelabel"));

    //gtk_label_set_text(GTK_LABEL(notiLabel2), (const char*)tmp);

    gtk_widget_show(roominfowindow);

    char tmp[255] = "";
    for(int i=0; i<rows; i++) {
        if(strcmp(name, PQgetvalue(res, i, 0))==0)
        {
            gtk_label_set_text(GTK_LABEL(roominfolabel), (const char*)name);
            gtk_label_set_text(GTK_LABEL(roomtypelabel), (const char*)PQgetvalue(res, i, 1));
            gtk_label_set_text(GTK_LABEL(daypricelabel), (const char*)PQgetvalue(res, i, 2));
            gtk_label_set_text(GTK_LABEL(hourlabel), (const char*)PQgetvalue(res, i, 3));
            strcat(tmp, PQgetvalue(res, i, 4));

            if(i==rows-1||strcmp(name, PQgetvalue(res, i+1, 0))!=0) break;
            strcat(tmp, ", ");
        }
    }
    gtk_label_set_text(GTK_LABEL(freeservicelabel), (const char*)tmp);

    PQclear(res);
    PQfinish(conn);
}

//Tan checkin 
/*----------------------------CHECKIN-----------------------------------*/
void on_checkinBtn_clicked()//khi an nut "Checkin"
{
    builder=gtk_builder_new_from_file("hotel.glade");
    checkinwindow=GTK_WIDGET(gtk_builder_get_object(builder,"checkinwindow"));
    checkinNameEntry = GTK_WIDGET(gtk_builder_get_object(builder,"checkinNameEntry"));
    checkinCMNDEntry = GTK_WIDGET(gtk_builder_get_object(builder,"checkinCMNDEntry"));
    checkinPhoneEntry = GTK_WIDGET(gtk_builder_get_object(builder,"checkinPhoneEntry"));
    checkinRoomEntry = GTK_WIDGET(gtk_builder_get_object(builder,"checkinRoomEntry"));

    chechkinAddBtn=GTK_WIDGET(gtk_builder_get_object(builder,"checkinAddBtn"));
    g_signal_connect(chechkinAddBtn, "clicked", G_CALLBACK(on_checkinadd_clicked), "NULL");
    checkinLabel=GTK_WIDGET(gtk_builder_get_object(builder,"checkinLabel"));
    reportLabel=GTK_WIDGET(gtk_builder_get_object(builder,"reportLabel"));
    gtk_widget_show(checkinwindow);
}

//Lenh sinh chuoi ngau nhien (do dai, ketqua)
void rand_text(int length, char *result) {
    int i, rand_int;
    char char_set[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz&quot";
 
    for (i = 0; i <length; i++) {
        result[i] = char_set[rand() % sizeof(char_set)];
    }
    result[length] = 0;
}

//lay today format 'yyyy-mm-dd'
void format_time(char *output){
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    sprintf(output, "%d-%d-%d",  
        timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday);
}


//Event khi an nut "Them"
void on_checkinadd_clicked()
{   
    char Key[10];
    char Name[50];
    char Phone[28];
    char CMND[28];
    char Room[200];
    sprintf(Name, "%s", gtk_entry_get_text(GTK_ENTRY(checkinNameEntry)));
    sprintf(Phone, "%s", gtk_entry_get_text(GTK_ENTRY(checkinPhoneEntry)));
    sprintf(CMND, "%s", gtk_entry_get_text(GTK_ENTRY(checkinCMNDEntry)));
    sprintf(Room, "%s", gtk_entry_get_text(GTK_ENTRY(checkinRoomEntry)));
    
    //Neu chua dien thong tin
    if((strlen(Name)==0) || (strlen(Phone)==0) || (strlen(CMND)==0) || (strlen(Room)==0) )
        {
            gtk_label_set_text(GTK_LABEL(checkinLabel), "HAY DIEN DU THONG TIN!\n");
            return;
        }

    //Phan SQL
    const char *conninfo;
    PGconn *conn; 
    conninfo = "host=localhost user=postgres password=root dbname=hotel";
    conn = PQconnectdb(conninfo);
    if(PQstatus(conn) != CONNECTION_OK)
    {
        printf("Connection to database failed %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }

//Lay MA PHONG voi TEN PHONG da nhap, Kiem tra TEN PHONG vua nhap co thoa man KHONG
    char cmdRoom[255]="select ma_phong from phong ph where ph.ten_phong=";
    strcat(cmdRoom,"(\'");
    strcat(cmdRoom, Room);
    strcat(cmdRoom,"\');");
    char *commandRoom=&cmdRoom[0];
    PGresult *resRoom = PQexec(conn, commandRoom);    
    resRoom = PQexec(conn, commandRoom); 
        if (PQresultStatus(resRoom) != PGRES_TUPLES_OK) {
            printf("Loi Lay MA PHONG\n");        
            PQclear(resRoom);
            do_exit(conn);
        }   
    int rowsRoom = PQntuples(resRoom);
    //Xem ten phong co ton tai hay khong
    if(rowsRoom==0){
        gtk_label_set_text(GTK_LABEL(checkinLabel), "PHONG DA NHAP KHONG TON TAI!\n");
        return;
    }
    char keyRoom[10];
    strcpy(keyRoom,PQgetvalue(resRoom,0,0));

    //Xem PHONG da nhap co trong PHONG KEM PHIEU THUE KHONG
    char cmdCheckRoom[255]="select * from phong_kem_phieu_thue pkpt where pkpt.ma_phong=";
    strcat(cmdCheckRoom,"(\'");
    strcat(cmdCheckRoom, keyRoom);
    strcat(cmdCheckRoom,"\');");
    char *commandCheckRoom=&cmdCheckRoom[0];
    PGresult *resCheckRoom = PQexec(conn, commandCheckRoom);    
    resCheckRoom = PQexec(conn, commandCheckRoom); 
        if (PQresultStatus(resCheckRoom) != PGRES_TUPLES_OK) {
            printf("Loi CHECK MA PHONG trong PHONG KIEM PHIEU THUE(checkin)\n");        
            PQclear(resCheckRoom);
            do_exit(conn);
        }   
    int rowsCheckRoom = PQntuples(resCheckRoom);
    for(int i=0; i<rowsCheckRoom; i++)
    {
        if(strlen(PQgetvalue(resCheckRoom, i, 3)) <= 0){
            gtk_label_set_text(GTK_LABEL(checkinLabel), "PHONG DA NHAP DA CO NGUOI THUE!\n");
            return;
        }
    }
//Lenh sinh ngau nhien
    srand(time(NULL));
//Xem CMND Khach Hang vua nhap da ton tai hay chua
    char cmdKH[255]="select * from khach_hang KH where KH.cmnd=";
    strcat(cmdKH,"(\'");
    strcat(cmdKH, CMND);
    strcat(cmdKH,"\');");
    char *commandKH=&cmdKH[0];
    PGresult *resKH = PQexec(conn, commandKH); 
    if (PQresultStatus(resKH) != PGRES_TUPLES_OK) {
        printf("Loi tim CMND KHACH HANG Checkin\n");        
        PQclear(resKH);
        do_exit(conn);
    } 
    int rowsKH = PQntuples(resKH);
    //Neu CMND cua KHACH HANG da ton tai 
    if(rowsKH>=1) strcpy(Key,PQgetvalue(resKH,0,0));
    //Neu CMND khach hang CHUA TON TAI, tao MA KHACH HANG MOI
    if(rowsKH==0)
        {
            char dsmaKH[1000][30];//Mang ki tu chua tat ca ma KH da co
            char cmdPre[255];
            PGresult *resAll;
            resAll = PQexec(conn, "select * from khach_hang"); 
                if (PQresultStatus(resAll) != PGRES_TUPLES_OK) {
                    printf("Loi select All KHACH HANG Checkin\n");        
                    PQclear(resAll);
                    do_exit(conn);
                }
            int rowsAll = PQntuples(resAll);

            for(int i=0; i<rowsAll; i++){
                strcpy(dsmaKH[i], PQgetvalue(resAll,i,0));
            }
            int check=1, i=0, count = 0;
            char maKHmoi[10] = "KH";
            char num[5];
            sprintf(num, "%d", rowsAll+1);
            for(int i=0; i<4-strlen(num); i++)
            {
                strcat(maKHmoi, "0");
            }
            strcat(maKHmoi, num);
            //Sinh ngau nhien MA KHACH HANG 
            //check = 0, MA KHACH HANG sinh ngau nhien 'khong trung' MA KHACH HANG da co
            //check = 1, MA KHACH HANG sinh ngau nhien 'trung' MA KHACH HANG da co
            // while(check==1){
            //     rand_text(6, maKHmoi);//sinh ngau nhien MA KHACH HANG MOI co 6 ki tu
            //     for(int j=0; j<rowsAll; j++){
            //         check = 0;
            //         if(strcmp(maKHmoi,dsmaKH[i])==0){//Neu ma KH sinh ngau nhien
            //             check=1;
            //             break;
            //             }
            //     }
            // }
            strcpy(Key, maKHmoi);
        //THEM vao BANG KHACH HANG
        char cmdAKH[255]="insert into khach_hang(ma_khach_hang, ho_ten, sdt, cmnd, ma_nhan_vien) values ";
        strcat(cmdAKH,"(\'");
        strcat(cmdAKH, Key);
        strcat(cmdAKH,"\',");
        strcat(cmdAKH,"\'");
        strcat(cmdAKH, Name);
        strcat(cmdAKH,"\',");
        strcat(cmdAKH,"\'");
        strcat(cmdAKH, Phone);
        strcat(cmdAKH,"\',");
        strcat(cmdAKH,"\'");
        strcat(cmdAKH, CMND);
        strcat(cmdAKH,"\',");
        strcat(cmdAKH,"\'NV0001\');");
        char *commandAKH=&cmdAKH[0];
        PGresult *resAKH = PQexec(conn, commandAKH);
    }
    char ngayCheckin[26];// Ngay den (NGAY HOM NAY)
    format_time(ngayCheckin); //format ngayCkeckin yyyy-mm-dd

//KIEM TRA XEM KHACH HANG CO PHIEU THUE HAY CHUA: KIEM TRA PHIEU THUE (KTPT)
    char cmdKTPT[255]="select * from phieu_thue PT where PT.ma_khach_hang=";
    strcat(cmdKTPT,"(\'");
    strcat(cmdKTPT, Key);
    strcat(cmdKTPT,"\');");
    char *commandKTPT=&cmdKTPT[0];
    PGresult *resKTPT = PQexec(conn, commandKTPT); 
    if (PQresultStatus(resKTPT) != PGRES_TUPLES_OK) {
        printf("Loi tim CMND KHACH HANG Checkin\n");        
        PQclear(resKTPT);
        do_exit(conn);
    } 
    int rowsKTPT = PQntuples(resKTPT);
    char KeyPT[12];
    //Neu khach hang co PHIEU THUE roi, lay ma phieu thue
    if(rowsKTPT>=1)
        strcpy(KeyPT,PQgetvalue(resKTPT,0,0));
    //Neu khach hang chua co PHIEU THUE, sinh ngau nhien MA PHIEU THUE, tao PHIEU THUE MOI
    if(rowsKTPT==0){
            //Mang ki tu chua tat ca MA PHIEU THUE da co
            char dsmaPT[1000][30];
            PGresult *resAllPT;
            resAllPT = PQexec(conn, "select * from phieu_thue"); 
                if (PQresultStatus(resAllPT) != PGRES_TUPLES_OK) {
                    printf("Loi select All PHIEU THUE Checkin\n");        
                    PQclear(resAllPT);
                    do_exit(conn);
                }
            int rowsAllPT = PQntuples(resAllPT);

            for(int i1=0; i1<rowsAllPT; i1++){
                strcpy(dsmaPT[i1], PQgetvalue(resAllPT,i1,0));
            }
            int check1=1, i1=0;
            char maPTmoi[10];
            //Sinh ngau nhien MA PHIEU THUE 
            //check = 0, MA PHIEU THUE sinh ngau nhien 'khong trung' MA PHIEU THUE da co
            //check = 1, MA PHIEU THUE sinh ngau nhien 'trung' MA PHIEU THUE da co
            while(check1==1){
                rand_text(6, maPTmoi);//sinh ngau nhien MA KHACH HANG MOI co 6 ki tu
                for(int j=0; j<rowsAllPT; j++){
                    check1 = 0;
                    if(strcmp(maPTmoi,dsmaPT[i1])==0){//Neu ma KH sinh ngau nhien
                        check1=1;
                        break;
                    }
                }
            }
            strcpy(KeyPT, maPTmoi);

        //MA PHIEU THUE se duoc sinh ngau nhien o TREN, them vao BANG PHIEU THUE

        char cmdAPT[255]="insert into phieu_thue(ma_phieu_thue, ma_khach_hang, ma_nhan_vien, ngay_lap, so_tien_da_thanh_toan) values ";
        strcat(cmdAPT,"(\'");
        strcat(cmdAPT, KeyPT);
        strcat(cmdAPT,"\',");
        strcat(cmdAPT,"\'");
        strcat(cmdAPT, Key);
        strcat(cmdAPT,"\',");
        strcat(cmdAPT,"\'");
        strcat(cmdAPT, "NV0001");
        strcat(cmdAPT,"\',");
        strcat(cmdAPT,"\'");
        strcat(cmdAPT, ngayCheckin);
        strcat(cmdAPT,"\',");
        strcat(cmdAPT,"0);");
        char *commandAPT=&cmdAPT[0];
        PGresult *resAPT = PQexec(conn, commandAPT); 
    }
    //THEM vao phong_kem_phieu_thue
    char cmdAPKPT[255]="insert into phong_kem_phieu_thue(ma_phieu_thue, ma_phong, ngay_den) values";
    strcat(cmdAPKPT,"(\'");
    strcat(cmdAPKPT, KeyPT);
    strcat(cmdAPKPT,"\',");
    strcat(cmdAPKPT,"\'");
    strcat(cmdAPKPT, keyRoom);
    strcat(cmdAPKPT,"\',");
    strcat(cmdAPKPT,"\'");
    strcat(cmdAPKPT, ngayCheckin);
    strcat(cmdAPKPT,"\');");

    char *commandAPKPT=&cmdAPKPT[0];
    PGresult *resAPKPT = PQexec(conn, commandAPKPT); 

    char final[255]="";
    strcat(final, "THEM THANH CONG!\nMA KHACH HANG LA: ");
    strcat(final, Key);
    gtk_label_set_text(GTK_LABEL(checkinLabel), final);
}
/*--------------------------------------------------------------------------*/

/*-----------------------CHECK OUT-----------------------------------------*/
//Tan check out
void on_checkoutBtn_clicked()
{
    builder=gtk_builder_new_from_file("hotel.glade");
    checkoutwindow=GTK_WIDGET(gtk_builder_get_object(builder,"checkoutwindow"));
    checkoutButton=GTK_WIDGET(gtk_builder_get_object(builder,"checkoutButton"));
    checkoutRoomEntry=GTK_WIDGET(gtk_builder_get_object(builder,"checkoutRoomEntry"));
    g_signal_connect(checkoutButton,"clicked",G_CALLBACK(on_checkoutButton_clicked),"NULL");
    checkoutLabel=GTK_WIDGET(gtk_builder_get_object(builder,"checkoutLabel"));
    gtk_widget_show(checkoutwindow);
}

void on_checkoutButton_clicked()
{
    char Room[200];
    sprintf(Room, "%s", gtk_entry_get_text(GTK_ENTRY(checkoutRoomEntry)));

    const char *conninfo;
    PGconn *conn; 
    conninfo = "host=localhost user=postgres password=root dbname=hotel";
    conn = PQconnectdb(conninfo);
    if(PQstatus(conn) != CONNECTION_OK)
    {
        printf("Connection to database failed %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }

//Lay MA PHONG voi TEN PHONG da nhap
    char cmdRoom[255]="select ma_phong from phong ph where ph.ten_phong=";
    strcat(cmdRoom,"(\'");
    strcat(cmdRoom, Room);
    strcat(cmdRoom,"\');");
    char *commandRoom=&cmdRoom[0];
    PGresult *resRoom = PQexec(conn, commandRoom);    
    resRoom = PQexec(conn, commandRoom); 
        if (PQresultStatus(resRoom) != PGRES_TUPLES_OK) {
            printf("Loi Lay MA PHONG(checkout)\n");        
            PQclear(resRoom);
            do_exit(conn);
        }   
    int rowsRoom = PQntuples(resRoom);
    //Xem ten phong co ton tai hay khong
    if(rowsRoom==0){
        gtk_label_set_text(GTK_LABEL(checkoutLabel), "PHONG DA NHAP KHONG TON TAI!\n");
        return;
    }
    char keyRoom[10];
    strcpy(keyRoom,PQgetvalue(resRoom,0,0));

//Xem PHONG da nhap co trong PHONG KEM PHIEU THUE KHONG
    char cmdCheckRoom[255]="select * from phong_kem_phieu_thue pkpt where pkpt.ma_phong=";
    strcat(cmdCheckRoom,"(\'");
    strcat(cmdCheckRoom, keyRoom);
    strcat(cmdCheckRoom,"\');");
    char *commandCheckRoom=&cmdCheckRoom[0];
    PGresult *resCheckRoom = PQexec(conn, commandCheckRoom);    
    if (PQresultStatus(resCheckRoom) != PGRES_TUPLES_OK) {
        printf("Loi CHECK MA PHONG trong PHONG KIEM PHIEU THUE (checkout)\n");        
        PQclear(resCheckRoom);
        do_exit(conn);
    }   
    int rowsCheckRoom = PQntuples(resCheckRoom);
    if(rowsCheckRoom ==0)
    {
        gtk_label_set_text(GTK_LABEL(checkoutLabel), "PHONG NAY KHONG CO NGUOI O!\n");
        return;
    }

    int count = 0;
    int num = 0;
    for(int i=0; i<rowsCheckRoom; i++)
    {
        printf("%s\n", PQgetvalue(resCheckRoom, i, 3));
        if(strlen(PQgetvalue(resCheckRoom, i, 3))==0){
            printf("Phong nay co nguoi o\n");
            num = i;
            break;
        }
        else count++;
    }
    printf("%d\n", count);
    if(count == rowsCheckRoom) 
    {
        gtk_label_set_text(GTK_LABEL(checkoutLabel), "PHONG NAY KHONG CO NGUOI O!\n");
        return;
    }
    char keyPT[20];
    char ngayDen[30];
    //Lay ma phieu thue
    strcpy(keyPT, PQgetvalue(resCheckRoom,num,0));
    strcpy(ngayDen, PQgetvalue(resCheckRoom,num,2));
    printf("|%s|\n",ngayDen);
    

    char ngayCheckout[26];// Ngay den (NGAY HOM NAY)
    format_time(ngayCheckout); //format ngayCkeckin yyyy-mm-dd
    printf("Sau khi format %s\n", ngayCheckout);

    //rowsCheckRoom = PQexec(conn, commandCheckRoom);

    char cmdTT[200]="select lp.thue_theo_ngay * DATE_PART('day', ";
    strcat(cmdTT,"\'");
    strcat(cmdTT, ngayCheckout);
    strcat(cmdTT,"\'");
    strcat(cmdTT,"::timestamp - ");
    strcat(cmdTT,"\'");
    strcat(cmdTT, ngayDen);
    strcat(cmdTT,"\'");
    strcat(cmdTT, "::timestamp) from phong ph, loai_phong lp where ph.ten_phong = ");
    strcat(cmdTT,"\'");
    strcat(cmdTT, Room);
    strcat(cmdTT,"\'");
    strcat(cmdTT," and ph.ma_loai_phong = lp.ma_loai_phong;");
    char *commandTT=&cmdTT[0];
    printf("|%s|\n", commandTT);
    PGresult *resTT = PQexec(conn, commandTT);    
    resTT = PQexec(conn, commandTT); 
        if (PQresultStatus(resTT) != PGRES_TUPLES_OK) {
            printf("Loi Thanh toan (checkout)\n");        
            PQclear(resTT);
            do_exit(conn);
        }    
    char TB[100]="Checkout Thanh cong!\n Tong tien: ";
    printf("OK\n");
    char Tien[20];
    strcpy(Tien,PQgetvalue(resTT,0,0));
    strcat(TB,Tien);
    gtk_label_set_text(GTK_LABEL(checkoutLabel), TB);
 
//Update PHONG o bang PHONG KEM PHIEU THUE
    char cmdXPKPT[255]="update phong_kem_phieu_thue set ngay_di = ";
    strcat(cmdXPKPT,"\'");
    strcat(cmdXPKPT, ngayCheckout);
    strcat(cmdXPKPT,"\'");
    strcat(cmdXPKPT, "where ma_phong = \'");
    strcat(cmdXPKPT, keyRoom);
    strcat(cmdXPKPT, "\';");
    
    char *commandXPKPT = &cmdXPKPT[0];

    PGresult *resXPKPT = PQexec(conn, commandXPKPT);

//Kiem tra PHIEU THUE da THANH TOAN HET HAY CHUA (Trong PHIEU THUE con PHONG NAO khong?)
    /*char cmdCheckPT[255]="select * from phong_kem_phieu_thue pkpt where pkpt.ma_phieu_thue=";
    strcat(cmdCheckPT,"(\'");
    strcat(cmdCheckPT, keyPT);
    strcat(cmdCheckPT,"\');");
    char *commandCheckPT=&cmdCheckPT[0];
    PGresult *resCheckPT = PQexec(conn, commandCheckPT);    
    resCheckPT = PQexec(conn, commandCheckPT); 
        if (PQresultStatus(resCheckPT) != PGRES_TUPLES_OK) {
            printf("Loi KIEM TRA PHIEU THUE ton tai trong PHONG KIEM PHIEU THUE hay khong (checkout)\n");        
            PQclear(resCheckPT);
            do_exit(conn);
        }   
    int rowsCheckPT = PQntuples(resCheckPT);

    if(rowsCheckPT==0 ){//Neu PHIEU THUE khong con phong nao, Xoa PHIEU THUE o bang PHIEU THUE
        char cmdPT[255]="delete from phieu_thue where ma_phieu_thue=";
        strcat(cmdPT,"\'");
        strcat(cmdPT, keyPT);
        strcat(cmdPT,"\';");
        char *commandPT=&cmdPT[0];
        PGresult *resPT = PQexec(conn, commandPT);  
        printf("|%s|\n",cmdPT);
    }*/
    return;
    
}

/*-------------------------------------------------------------------------*/
/*-----------------------Xuat hoa don Dich vu-----------------------------------------*/
//Tan, Hien thi hoa don
void on_servicebillBtn_clicked()
{
    builder=gtk_builder_new_from_file("hotel.glade");
    servicebillwindow=GTK_WIDGET(gtk_builder_get_object(builder,"servicebillwindow"));
    servicebillEntry=GTK_WIDGET(gtk_builder_get_object(builder,"servicebillEntry"));
    searchbillButton=GTK_WIDGET(gtk_builder_get_object(builder,"searchbillButton")); 
    g_signal_connect(searchbillButton, "clicked", G_CALLBACK(on_searchbillButton_clicked), "NULL"); 
    billButton= GTK_WIDGET(gtk_builder_get_object(builder,"billButton"));
    gtk_widget_set_sensitive(billButton, FALSE);
    g_signal_connect(billButton,"clicked",G_CALLBACK(on_billButton_clicked),"NULL");

    namecustomerinbillLabel=GTK_WIDGET(gtk_builder_get_object(builder,"namecustomerinbillLabel"));
    serviceinbillLabel=GTK_WIDGET(gtk_builder_get_object(builder,"serviceinbillLabel"));
    servicepriceinbillLabel=GTK_WIDGET(gtk_builder_get_object(builder,"servicepriceinbillLabel"));
    totalpriceLabel=GTK_WIDGET(gtk_builder_get_object(builder,"totalpriceLabel"));
    gtk_widget_show(servicebillwindow);
}

void on_searchbillButton_clicked()
{
    //Lay ma khach hang
    char customerkey[50];
    sprintf(customerkey, "%s", gtk_entry_get_text(GTK_ENTRY(servicebillEntry)));
    //Phan SQL
    const char *conninfo;
    PGconn *conn; 
    conninfo = "host=localhost user=postgres password=root dbname=hotel";
    conn = PQconnectdb(conninfo);
    if(PQstatus(conn) != CONNECTION_OK)
    {
        printf("Connection to database failed %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }
//Tim Khach hang theo ma khach hang
    char cmdKH[255]="select kh.ho_ten from khach_hang kh where kh.ma_khach_hang =";
    strcat(cmdKH,"\'");
    strcat(cmdKH, customerkey);
    strcat(cmdKH,"\'");
    char *commandKH=&cmdKH[0];
    PGresult *resKH = PQexec(conn, commandKH); 
    if (PQresultStatus(resKH) != PGRES_TUPLES_OK) {
        printf("No room name retrieved\n");        
        PQclear(resKH);
        do_exit(conn);
    } 

    int rowsKH = PQntuples(resKH);
    if(rowsKH==0)
        {
            gtk_label_set_text(GTK_LABEL(serviceinbillLabel), "KHONG CO\n");
            gtk_label_set_text(GTK_LABEL(servicepriceinbillLabel), "MAKH!\n");
            gtk_label_set_text(GTK_LABEL(namecustomerinbillLabel), "Khong tim thay Ma KH!");
            gtk_widget_set_sensitive(billButton, FALSE);
            return;
        }
    else
    {
        char nameKH[60]="";
        strcat(nameKH, PQgetvalue(resKH, 0, 0));

        gtk_label_set_text(GTK_LABEL(namecustomerinbillLabel), (char*)nameKH);;
    }
//Xuat hoa don
    char cmd[256]="select hd.*, dv.ten_dich_vu, dv.don_gia from hoa_don hd, lich_su_su_dungdv ls, dich_vu dv where (hd.ma_hoa_don=ls.ma_hoa_don) and (ls.ma_dich_vu=dv.ma_dich_vu) and hd.ma_khach_hang = ";
    strcat(cmd,"\'");
    strcat(cmd, customerkey);
    strcat(cmd,"\'");
    char *command=&cmd[0];
    PGresult *res = PQexec(conn, command); 
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("No data customer\n");        
        PQclear(res);
        do_exit(conn);
    }    

    int rows = PQntuples(res);

    if(rows==0)
    {
        gtk_label_set_text(GTK_LABEL(serviceinbillLabel), "KHONG CO!\n");
        gtk_label_set_text(GTK_LABEL(servicepriceinbillLabel), "KHONG CO!\n");
        gtk_label_set_text(GTK_LABEL(totalpriceLabel), "KHONG CO!\n");
        gtk_widget_set_sensitive(billButton, FALSE);
    }
    else
    {
        //Hien cac dich vu da su dung
        char service_tmp[255] = "";
        char price_tmp[255] = "";
        char totalbill[255]="";
        for(int i=0; i<rows; i++)
        {
            strcat(service_tmp, PQgetvalue(res, i, 4));
            strcat(service_tmp, "\n");

            strcat(price_tmp, PQgetvalue(res, i, 5));
            strcat(price_tmp, "\n");  
        }
        strcat(totalbill, PQgetvalue(res, 0, 3));
        gtk_label_set_text(GTK_LABEL(totalpriceLabel), (const char*)totalbill);
        gtk_label_set_text(GTK_LABEL(serviceinbillLabel), (const char*)service_tmp);
        gtk_label_set_text(GTK_LABEL(servicepriceinbillLabel), (const char*)price_tmp);
        gtk_widget_set_sensitive(billButton, TRUE);
    }
}

void on_billButton_clicked()
{
    //Lay ma khach hang
    char customerkey[8];
    sprintf(customerkey, "%s", gtk_entry_get_text(GTK_ENTRY(servicebillEntry)));
    //Phan SQL connection
    const char *conninfo;
    PGconn *conn; 
    conninfo = "host=localhost user=postgres password=root dbname=hotel";
    conn = PQconnectdb(conninfo);
    if(PQstatus(conn) != CONNECTION_OK)
    {
        printf("Connection to database failed %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }
    //Phan viet lenh SQL delete
    //Xoa o bang Lich su su dung dich vu
    char cmd[256]="delete from lich_su_su_dungdv where lich_su_su_dungdv.ma_khach_hang=";
    strcat(cmd,"\'");
    strcat(cmd, customerkey);
    strcat(cmd,"\'");
    char *command=&cmd[0];
    PGresult *res = PQexec(conn, command);  

    //xoa o trong bang Hoa don
    char cmd1[256]="delete from hoa_don where hoa_don.ma_khach_hang=";
    strcat(cmd1,"\'");
    strcat(cmd1, customerkey);
    strcat(cmd1,"\'");
    char *command1=&cmd1[0];
    PGresult *res1 = PQexec(conn, command1); 

    gtk_label_set_text(GTK_LABEL(serviceinbillLabel), "DA THANH TOAN!\n");
    gtk_label_set_text(GTK_LABEL(servicepriceinbillLabel), "THANH CONG!\n");
    gtk_label_set_text(GTK_LABEL(totalpriceLabel), "0\n");
    gtk_widget_set_sensitive(billButton, FALSE);      
}
/*--------------------------------------------------------------------------*/

//Duy
void on_emptyroomwindow_destroy()                     //destroy
{
    gtk_widget_destroy(emptyroomwindow);
    gtk_widget_set_sensitive(emptyBtn, TRUE);
}
void on_emptyBtn_clicked()                            
{
    gtk_widget_set_sensitive(emptyBtn, FALSE);
    builder = gtk_builder_new_from_file("hotel.glade");
    emptyroomwindow = GTK_WIDGET(gtk_builder_get_object(builder, "emptyroomwindow"));
    g_signal_connect(emptyroomwindow, "destroy", G_CALLBACK(on_emptyroomwindow_destroy), "NULL");
    emptyroomview = GTK_TREE_VIEW(gtk_builder_get_object(builder, "emptyroomview"));
    emptyroomselection = GTK_TREE_SELECTION(gtk_builder_get_object(builder, "emptyroomselection"));
    GtkListStore *emptyroom = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);
    //kiem tra ket noi
    const char *conninfo;
    PGconn *conn; 
    conninfo = "host=localhost user=postgres password=root dbname=hotel";
    conn = PQconnectdb(conninfo);
    if(PQstatus(conn) != CONNECTION_OK)
    {
        printf("Connection to database failed %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }
    printf("So far so good\n");
    //Cau truy van current date neu > ngay den va nho hon ngay di => cac phong dang co nguoi o . chi can lay ra cac phong ma ma phong khong trung voi phong co nguoi o
    // se la phong trong
    PGresult *res = PQexec(conn, "SELECT phong.ten_phong, khach_hang.ho_ten, loai_phong.loai_phong, phong_kem_phieu_thue.ngay_di FROM phong LEFT JOIN loai_phong using (ma_loai_phong) LEFT JOIN phong_kem_phieu_thue using(ma_phong) LEFT JOIN phieu_thue using(ma_phieu_thue) LEFT JOIN khach_hang using(ma_khach_hang)"); 
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("No service data retrieved\n");        
        PQclear(res);
    } 
    int rows = PQntuples(res);
    
    for(int i=0; i<rows; i++) 
    {
        if(strlen(PQgetvalue(res, i, 1)) == 0 || strlen(PQgetvalue(res, i, 3))>0) 
        {
            gtk_list_store_append(emptyroom, &roomiter);
            gtk_list_store_set(emptyroom, &roomiter, 0, PQgetvalue(res, i, 0), 1, PQgetvalue(res, i, 2), -1);
        }
    }
    gtk_tree_view_set_model(GTK_TREE_VIEW(emptyroomview), GTK_TREE_MODEL(emptyroom));   
    GtkTreeViewColumn *col1, *col2;
    GtkCellRenderer *renderer;
    GtkTreeModel *model = gtk_tree_view_get_model(emptyroomview);
    renderer = gtk_cell_renderer_text_new();
    col1 = gtk_tree_view_column_new_with_attributes("Ten phong", renderer, "text", 0, NULL);
    col2 = gtk_tree_view_column_new_with_attributes("Loai phong", renderer, "text", 1, NULL);
    
    gtk_tree_view_append_column(emptyroomview, col1);
    gtk_tree_view_append_column(emptyroomview, col2);
    gtk_widget_show(emptyroomwindow);
    PQclear(res);
    PQfinish(conn);
}
void on_roomsearchwindow_destroy()
{
    gtk_widget_destroy(roomsearchwindow);
    gtk_widget_set_sensitive(searchBtn, TRUE);
}
void on_roomsearchwindow2_destroy()
{
    gtk_widget_destroy(roomsearchwindow2);
    gtk_widget_set_sensitive(roomsearchOkBtn, TRUE);
}
void on_lpks_destroy()
{
    gtk_widget_destroy(lpks);
    gtk_widget_set_sensitive(searchBtn, TRUE);   
}
void on_searchBtn_clicked()    //khi bam vao nut tim phong tren menu
{
    //hien thi cua so de nhap loai phong
    builder = gtk_builder_new_from_file("hotel.glade");
    roomsearchwindow = GTK_WIDGET(gtk_builder_get_object(builder, "roomsearchwindow"));
    g_signal_connect(roomsearchwindow, "destroy", G_CALLBACK(on_roomsearchwindow_destroy), "NULL");
    gtk_builder_connect_signals(builder, NULL);
    roomsearchEntry = GTK_WIDGET(gtk_builder_get_object(builder, "roomsearchEntry"));
    roomsearchFixed = GTK_WIDGET(gtk_builder_get_object(builder, "roomsearchFixed"));
    notiLabel2 = GTK_WIDGET(gtk_builder_get_object(builder, "notiLabel2"));
    roomsearchOkBtn = GTK_WIDGET(gtk_builder_get_object(builder, "roomsearchOkBtn"));

    //hien thi cua so cac loai phong de nhan vien co the quan sat.
    //kiem tra ket noi
    const char *conninfo;
    PGconn *conn; 
    conninfo = "host=localhost user=postgres password=root dbname=hotel";
    conn = PQconnectdb(conninfo);
    if(PQstatus(conn) != CONNECTION_OK)
    {
        printf("Connection to database failed %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }
    PGresult *res = PQexec(conn, "SELECT ma_loai_phong, loai_phong FROM loai_phong"); 
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("No data retrieved\n");        
        PQclear(res);
        do_exit(conn);
    } 
    int rows = PQntuples(res);
    gtk_widget_set_sensitive(searchBtn, FALSE);   
    lpks = GTK_WIDGET(gtk_builder_get_object(builder, "lpks"));
    g_signal_connect(lpks, "destroy", G_CALLBACK(on_lpks_destroy), "NULL");
    lpksview = GTK_TREE_VIEW(gtk_builder_get_object(builder, "lpksview"));
    lpksselection = GTK_TREE_SELECTION(gtk_builder_get_object(builder, "lpksselection"));
    GtkListStore *lpkslist = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);
    int i;
    //cho du lieu vao lpkslist de in ra
    for(i = 0; i < rows; i++)
    {
        gtk_list_store_append(lpkslist, &lpksIter);
        gtk_list_store_set(lpkslist, &lpksIter, 0, PQgetvalue(res, i, 0), 1, PQgetvalue(res, i, 1), -1);
    }
    gtk_tree_view_set_model(GTK_TREE_VIEW(lpksview), GTK_TREE_MODEL(lpkslist));   
    GtkTreeViewColumn *col1, *col2;
    GtkCellRenderer *renderer;
    GtkTreeModel *model = gtk_tree_view_get_model(lpksview);
    renderer = gtk_cell_renderer_text_new();
    col1 = gtk_tree_view_column_new_with_attributes("ma loai phong", renderer, "text", 0, NULL);
    col2 = gtk_tree_view_column_new_with_attributes("loai phong", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(lpksview, col1);
    gtk_tree_view_append_column(lpksview, col2);
    gtk_widget_show(roomsearchwindow);
    gtk_widget_show(lpks);
    PQclear(res);
    PQfinish(conn);
}
void on_roomsearchOkBtn_clicked()
{   
    //doc loai phong nguoi dung nhap vao
    char roomtype[50];
    sprintf(roomtype, "%s", gtk_entry_get_text(GTK_ENTRY(roomsearchEntry)));
    //kiem tra ket noi database
    const char *conninfo;
    PGconn *conn; 
    conninfo = "host=localhost user=postgres password=root dbname=hotel";
    conn = PQconnectdb(conninfo);
    if(PQstatus(conn) != CONNECTION_OK)
    {
        printf("Connection to database failed %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }
    PGresult *res = PQexec(conn, "SELECT ma_loai_phong FROM loai_phong"); 
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("No data retrieved\n");        
        PQclear(res);
        do_exit(conn);
    }    
    int rows = PQntuples(res);
    int count = 0;
    //kiem tra xem ma loai phong nhap co dung ko
    for(int i=0; i<rows; i++) {
        //printf("%s%s\n", PQgetvalue(res, i, 0), roomtype);
        if(strcmp(roomtype, PQgetvalue(res, i, 0))==0)
        {
            count++;
            //printf("Found info\n");
            break;
        }
    }
    // neu nhap sai, in thong bao
    if(count<=0) gtk_label_set_text(GTK_LABEL(notiLabel2), (const gchar*)"Sai loai phong");
    else
    {
        gtk_widget_set_sensitive(roomsearchOkBtn , FALSE);
        builder = gtk_builder_new_from_file("hotel.glade");
        roomsearchwindow2 = GTK_WIDGET(gtk_builder_get_object(builder, "roomsearchwindow2"));
        g_signal_connect(roomsearchwindow2, "destroy", G_CALLBACK(on_roomsearchwindow2_destroy), "NULL");
        roomsearchwindow2view = GTK_TREE_VIEW(gtk_builder_get_object(builder, "roomsearchwindow2view"));
        roomsearchwindow2selection = GTK_TREE_SELECTION(gtk_builder_get_object(builder, "roomsearchwindow2selection"));

        GtkListStore *searchroom = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);   
        
        //cau truy van nay giong het cai phong trong o tren
        PGresult *res2 = PQexec(conn, "select * from phong"); 
        int rows2 = PQntuples(res2);
        if (PQresultStatus(res2) != PGRES_TUPLES_OK) {
            printf("No service data retrieved\n");        
            PQclear(res2);
        }    
        int i;
        //cho du lieu vao search room neu ma phong trong danh sach phong trong giong voi roomtype. KHONG CHO MA PHONG VAO.
        for(i=0; i<rows2; i++) {
            if(strcmp(PQgetvalue(res2,i,2), roomtype) == 0 )              //dieu kien
            {   
                printf("Found the required room\n");
                gtk_list_store_append(searchroom, &roomsearchiter);
                gtk_list_store_set(searchroom, &roomsearchiter, 0, PQgetvalue(res2, i, 0), 1, PQgetvalue(res2, i, 1), -1);
            }
        }
        
        gtk_tree_view_set_model(GTK_TREE_VIEW(roomsearchwindow2view), GTK_TREE_MODEL(searchroom));   
        GtkTreeViewColumn *col1, *col2;
        GtkCellRenderer *renderer;
        GtkTreeModel *model = gtk_tree_view_get_model(roomsearchwindow2view);
        renderer = gtk_cell_renderer_text_new();
        col1 = gtk_tree_view_column_new_with_attributes("Ma phong", renderer, "text", 0, NULL);
        col2 = gtk_tree_view_column_new_with_attributes("Ten phong", renderer, "text", 1, NULL);
        gtk_tree_view_append_column(roomsearchwindow2view, col1);
        gtk_tree_view_append_column(roomsearchwindow2view, col2);
        gtk_widget_show(roomsearchwindow2);
        PQclear(res);
        PQfinish(conn);
        PQclear(res2);
    }
}
void on_roombillWindow1_destroy()
{
    gtk_widget_destroy(roombillWindow1);
    gtk_widget_set_sensitive(roombillBtn, TRUE);  
}
void on_roombillWindow2_destroy()
{
    gtk_widget_destroy(roombillWindow2);
    gtk_widget_set_sensitive(CusIDokBtn, TRUE);  
}

void on_roombillBtn_clicked()
{
    builder = gtk_builder_new_from_file("hotel.glade");
    roombillWindow1 = GTK_WIDGET(gtk_builder_get_object(builder, "roombillWindow1"));
    g_signal_connect(roombillWindow1, "destroy", G_CALLBACK(on_roombillWindow1_destroy), "NULL");
    gtk_builder_connect_signals(builder, NULL);
    CusIDEntry = GTK_WIDGET(gtk_builder_get_object(builder, "CusIDEntry"));
    roombillWindow1Fixed = GTK_WIDGET(gtk_builder_get_object(builder, "roombillWindow1Fixed"));
    notiRoombill = GTK_WIDGET(gtk_builder_get_object(builder, "notiRoombill"));
    CusIDokBtn = GTK_WIDGET(gtk_builder_get_object(builder, "CusIDokBtn"));
    roomnameEntry = GTK_WIDGET(gtk_builder_get_object(builder, "roomnameEntry"));
    arrivalEntry = GTK_WIDGET(gtk_builder_get_object(builder, "arrivalEntry"));
    const char *holder = "yyyy-mm-dd";
    gtk_entry_set_placeholder_text (GTK_ENTRY(arrivalEntry), holder);
    departureEntry = GTK_WIDGET(gtk_builder_get_object(builder, "departureEntry"));
    gtk_entry_set_placeholder_text (GTK_ENTRY(departureEntry), holder);
    gtk_widget_show(roombillWindow1);
}
void on_CusIDokBtn_clicked()
{
    char cusname[30];
    char roomname[50];
    char arrival[15];
    char departure[15];
    sprintf(cusname, "%s", gtk_entry_get_text(GTK_ENTRY(CusIDEntry)));
    sprintf(roomname, "%s", gtk_entry_get_text(GTK_ENTRY(roomnameEntry)));
    sprintf(arrival, "%s", gtk_entry_get_text(GTK_ENTRY(arrivalEntry)));
    sprintf(departure, "%s", gtk_entry_get_text(GTK_ENTRY(departureEntry)));
    
    printf("ten khach la %s\n", cusname);
    printf("ten phong la %s\n", roomname);
    printf("ngay den la %s\n", arrival);
    printf("ngay di la %s\n", departure);
    //kiem tra ket noi database
    const char *conninfo;
    PGconn *conn; 
    conninfo = "host=localhost user=postgres password=root dbname=hotel";
    conn = PQconnectdb(conninfo);
    if(PQstatus(conn) != CONNECTION_OK)
    {
        printf("Connection to database failed %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        exit(1);
    }
    /*
    strcat(tmp, Cusname);
    strcat(tmp,"\' AND phong.ten_phong = \'");
    strcat(tmp, roomname);
    strcat(tmp,"\' AND phong_kem_phieu_thue.ngay_den = \'");
    strcat(tmp, arrival);
    strcat(tmp,"\' AND phong_kem_phieu_thue.ngay_di = \'");
    strcat(tmp, departure);
    strcat(tmp,"\';");*/
    //printf("%s\n", (const char*)"SELECT khach_hang.ho_ten, khach_hang.cmnd, phong.ten_phong, phong_kem_phieu_thue.ngay_den, phong_kem_phieu_thue.ngay_di, (loai_phong.thue_theo_ngay * (phong_kem_phieu_thue.ngay_di::date - phong_kem_phieu_thue.ngay_den::date)) as tongtien FROM khach_hang LEFT JOIN phieu_thue using(ma_khach_hang) LEFT JOIN phong_kem_phieu_thue using(ma_phieu_thue) LEFT JOIN phong using (ma_phong) LEFT JOIN loai_phong using (ma_loai_phong)");
    
    PGresult *res = PQexec(conn, (const char*)"SELECT khach_hang.ho_ten, khach_hang.cmnd, phong.ten_phong, phong_kem_phieu_thue.ngay_den, phong_kem_phieu_thue.ngay_di, (loai_phong.thue_theo_ngay * (phong_kem_phieu_thue.ngay_di::date - phong_kem_phieu_thue.ngay_den::date)) as tongtien FROM khach_hang LEFT JOIN phieu_thue using(ma_khach_hang) LEFT JOIN phong_kem_phieu_thue using(ma_phieu_thue) LEFT JOIN phong using (ma_phong) LEFT JOIN loai_phong using (ma_loai_phong)"); 
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        printf("No data retrieved\n");        
        PQclear(res);
        do_exit(conn);
    } 
    int rows = PQntuples(res);
    int count = 0;
    
    printf("So far so good room bill\n");

    
    
    if(rows<=0) gtk_label_set_text(GTK_LABEL(notiRoombill), (const gchar*)"Sai ten khach hoac ten phong hoac thoi gian");
    else
    {
        printf("okk");
        builder = gtk_builder_new_from_file("hotel.glade");
        roombillWindow2 = GTK_WIDGET(gtk_builder_get_object(builder, "roombillWindow2"));
        g_signal_connect(roombillWindow2, "destroy", G_CALLBACK(on_roombillWindow2_destroy), "NULL");
        roombillnamelabel = GTK_WIDGET(gtk_builder_get_object(builder, "roombillnamelabel"));
        cmndlabel = GTK_WIDGET(gtk_builder_get_object(builder, "cmndlabel"));
        roombillroomlabel = GTK_WIDGET(gtk_builder_get_object(builder, "roombillroomlabel"));
        arrivallabel = GTK_WIDGET(gtk_builder_get_object(builder, "arrivallabel"));
        departurelabel = GTK_WIDGET(gtk_builder_get_object(builder, "departurelabel"));
        roombillmoneylabel = GTK_WIDGET(gtk_builder_get_object(builder, "roombillmoneylabel"));
        //PGresult *res3 = PQexec(conn, "select b1.ho_ten, b1.cmnd , b2.ten_phong, ptk.ngay_den , ptk.ngay_di, b2.tongtien as bill from (select kh.ho_ten , kh.ma_khach_hang, kh.cmnd, kh.sdt, pt.ma_phieu_thue from khach_hang kh, phieu_thue pt where kh.ma_khach_hang = pt.ma_khach_hang) as b1, (select ptk.ma_phieu_thue, ptk.ma_phong,p.ten_phong, lp.ma_loai_phong, lp.thue_theo_ngay, (ptk.ngay_di::date - ptk.ngay_den::date) AS days, (lp.thue_theo_ngay * (ptk.ngay_di::date - ptk.ngay_den::date)) as tongtien from phong_kem_phieu_thue ptk, loai_phong lp, phong p where ptk.ma_phong = p.ma_phong and p.ma_loai_phong = lp.ma_loai_phong) as b2, phong_kem_phieu_thue ptk where b1.ma_phieu_thue = b2.ma_phieu_thue and b2.ma_phieu_thue = ptk.ma_phieu_thue;");
        //int rows3 = PQntuples(res3);
        if (PQresultStatus(res) != PGRES_TUPLES_OK) {
            printf("%d", rows);
            printf("No service data retrieved\n");        
            PQclear(res);
        }    
        int i;
        printf("okay cool\n");
        for(i=0; i<rows; i++)
        {
            /*printf("%s%s\n", PQgetvalue(res, i, 0), cusname);
            printf("%d\n", strcmp(cusname, PQgetvalue(res, i, 0)));
            printf("%s%s\n", roomname, PQgetvalue(res, i, 2));
            printf("%d\n", strcmp(roomname, PQgetvalue(res, i, 2)));
            //printf("%s\n", arrival);
            printf("%s%s\n", arrival, PQgetvalue(res, i, 3));
            printf("%d\n", strcmp(arrival, PQgetvalue(res, i, 3)));
            printf("%s%s\n", departure, PQgetvalue(res, i, 4));
            printf("%d\n", strcmp(departure, PQgetvalue(res, i, 4)));
            */
            
            //if(strcmp(PQgetvalue(res, 0, 0), PQgetvalue(res3, i, 0))==0 && strcmp(PQgetvalue(res, 0, 1), PQgetvalue(res3, i, 2))==0)
            if(strcmp(cusname, PQgetvalue(res, i, 0))==0 && strcmp(roomname, PQgetvalue(res, i, 2))==0 && strcmp(arrival, PQgetvalue(res, i, 3))==0 && strcmp(departure, PQgetvalue(res, i, 4))==0)
            {
                gtk_widget_set_sensitive(CusIDokBtn, FALSE);
                gtk_widget_show(roombillWindow2);
                gtk_label_set_text(GTK_LABEL(roombillnamelabel), (const char*)PQgetvalue(res, i, 0));
                gtk_label_set_text(GTK_LABEL(cmndlabel), (const char*)PQgetvalue(res, i, 1));
                gtk_label_set_text(GTK_LABEL(roombillroomlabel), (const char*)PQgetvalue(res, i, 2));
                gtk_label_set_text(GTK_LABEL(arrivallabel), (const char*)PQgetvalue(res, i, 3));
                gtk_label_set_text(GTK_LABEL(departurelabel), (const char*)PQgetvalue(res, i, 4));
                gtk_label_set_text(GTK_LABEL(roombillmoneylabel), (const char*)PQgetvalue(res, i, 5));

                break;         
            }
        }
        PQclear(res);
        //PQclear(res3);
        PQfinish(conn);
    }
}

void delete_char(char *A)
{
    A[strlen(A) - 1] = '\0';
}