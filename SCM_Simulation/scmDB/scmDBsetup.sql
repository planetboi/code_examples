CREATE DATABASE IF NOT EXISTS `scmDB`
CHARACTER SET utf8
COLLATE utf8_unicode_ci;

USE scmDB;

SET NAMES utf8mb4;

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


CREATE TABLE `sensorInformation` (
   `sensorID`               INT                   NOT NULL,
   `sensorType`           VARCHAR(20)             NOT NULL,
   `scannedTime`          VARCHAR(50)             NOT NULL,
   `scannedProduct`       VARCHAR(20)             NOT NULL,
   `messageID`              INT                   NOT NULL AUTO_INCREMENT,
   PRIMARY KEY (`messageID`))
DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

  COMMIT;