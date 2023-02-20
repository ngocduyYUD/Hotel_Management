create database khachsandatabase;
\c khachsandatabase;
create table khach_hang
(
	ma_khach_hang char(6) not null primary key,
	ho_ten varchar(30) not null,
	SDT char(14) not null,
	CMND varchar(15) not null,
	ma_nhan_vien char(6) not null
);
create table nhan_vien
(
	ma_nhan_vien char(6) not null primary key,
	ho_ten varchar(30) not null,
	SDT char(14) not null,
	mat_khau varchar(30) not null
);
create table phieu_thue
(
	ma_phieu_thue char(6) not null primary key,
	ma_khach_hang char(6) not null,
	ma_nhan_vien char(6) not null,
	ngay_lap DATE,
	so_tien_da_thanh_toan int,
	CONSTRAINT ptfk1 foreign key(ma_khach_hang) REFERENCES khach_hang(ma_khach_hang),
	CONSTRAINT ptfk2 foreign key(ma_nhan_vien) REFERENCES nhan_vien(ma_nhan_vien)
);
create table phong_kem_phieu_thue
(
	ma_phieu_thue char(6) not null,
	ma_phong char(6) not null,
	ngay_den date,
	ngay_di date,
	constraint pkptpk primary key(ma_phieu_thue, ma_phong)
);
create table phong
(
	ma_phong char(6) not null primary key,
	ten_phong varchar(8),
	ma_loai_phong char(6) not null
);
CREATE table loai_phong
(
	ma_loai_phong char(6) not null primary key,
	loai_phong varchar(15),
	thue_theo_ngay int,
	thue_theo_gio int
);
create table hoa_don
(
	ma_hoa_don char(6) not null primary key,
	ma_khach_hang char(6) not null,
	ngay_lap DATE not null,
	tong_tien int not null,
	CONSTRAINT hdfk FOREIGN key (ma_khach_hang) REFERENCES khach_hang(ma_khach_hang)
);
create table dich_vu
(	
	ma_dich_vu char(6) not null primary key,
	ten_dich_vu varchar(30),
	don_gia int
);
create table lich_su_su_dungdv
(
	ma_dich_vu char(6) not null,
	ma_hoa_don char(6) not null,
	ma_khach_hang char(6) not null,
	constraint lsfk1 foreign key(ma_dich_vu) REFERENCES dich_vu(ma_dich_vu),
	constraint lsfk2 foreign key(ma_hoa_don) REFERENCES hoa_don(ma_hoa_don),
	constraint lsfk3 foreign key(ma_khach_hang) REFERENCES khach_hang(ma_khach_hang)
);
create table dich_vu_free
(
	ma_loai_phong char(6) not null,
	ma_dich_vu char(6) not null,
	CONSTRAINT dvfpk PRIMARY key(ma_loai_phong, ma_dich_vu),
	constraint dvffk1 FOREIGN key(ma_loai_phong) REFERENCES loai_phong(ma_loai_phong),
	constraint dvffk2 FOREIGN key(ma_dich_vu) REFERENCES dich_vu(ma_dich_vu)
);

ALTER table khach_hang add constraint khfk foreign key (ma_nhan_vien) REFERENCES nhan_vien(ma_nhan_vien);
alter table phong_kem_phieu_thue add constraint pkptfk1 FOREIGN key (ma_phieu_thue) REFERENCES phieu_thue(ma_phieu_thue);
alter table phong_kem_phieu_thue add constraint pkptfk2 FOREIGN key (ma_phong) REFERENCES phong(ma_phong);
alter table phong add constraint pfk FOREIGN key (ma_loai_phong) REFERENCES loai_phong(ma_loai_phong);

