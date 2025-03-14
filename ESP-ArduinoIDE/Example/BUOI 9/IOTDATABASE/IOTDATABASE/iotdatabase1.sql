-- phpMyAdmin SQL Dump
-- version 5.0.3
-- https://www.phpmyadmin.net/
--
-- Máy chủ: 127.0.0.1
-- Thời gian đã tạo: Th2 02, 2021 lúc 03:01 PM
-- Phiên bản máy phục vụ: 10.4.14-MariaDB
-- Phiên bản PHP: 7.4.11

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Cơ sở dữ liệu: `iotdatabase1`
--

-- --------------------------------------------------------

--
-- Cấu trúc bảng cho bảng `dbdieukhien`
--

CREATE TABLE `dbdieukhien` (
  `id` int(11) NOT NULL,
  `tb1` varchar(100) NOT NULL,
  `tb2` varchar(100) NOT NULL,
  `cai1` varchar(100) NOT NULL,
  `cai2` varchar(100) NOT NULL,
  `date` date NOT NULL,
  `time` time NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Đang đổ dữ liệu cho bảng `dbdieukhien`
--

INSERT INTO `dbdieukhien` (`id`, `tb1`, `tb2`, `cai1`, `cai2`, `date`, `time`) VALUES
(1, '0', '0', '300', '500', '2021-02-02', '14:16:31');

-- --------------------------------------------------------

--
-- Cấu trúc bảng cho bảng `dbsensor`
--

CREATE TABLE `dbsensor` (
  `id` int(11) NOT NULL,
  `nd` varchar(100) NOT NULL,
  `da` varchar(100) NOT NULL,
  `tb1` varchar(100) NOT NULL,
  `tb2` varchar(100) NOT NULL,
  `cai1` varchar(100) NOT NULL,
  `cai2` varchar(100) NOT NULL,
  `date` date NOT NULL,
  `time` time NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Đang đổ dữ liệu cho bảng `dbsensor`
--

INSERT INTO `dbsensor` (`id`, `nd`, `da`, `tb1`, `tb2`, `cai1`, `cai2`, `date`, `time`) VALUES
(1, '1', '2', '3', '4', '5', '6', '2021-02-02', '20:16:39'),
(2, '11', '22', '33', '44', '55', '66', '2021-02-02', '20:17:07'),
(3, '', '', '', '', '', '', '2021-02-02', '20:36:14'),
(4, '', '', '', '', '', '', '2021-02-02', '20:36:15'),
(5, '', '', '', '', '', '', '2021-02-02', '20:36:16'),
(6, '', '', '', '', '', '', '2021-02-02', '20:36:18'),

--
-- Chỉ mục cho các bảng đã đổ
--

--
-- Chỉ mục cho bảng `dbdieukhien`
--
ALTER TABLE `dbdieukhien`
  ADD PRIMARY KEY (`id`);

--
-- Chỉ mục cho bảng `dbsensor`
--
ALTER TABLE `dbsensor`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT cho các bảng đã đổ
--

--
-- AUTO_INCREMENT cho bảng `dbdieukhien`
--
ALTER TABLE `dbdieukhien`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;

--
-- AUTO_INCREMENT cho bảng `dbsensor`
--
ALTER TABLE `dbsensor`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=885;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
