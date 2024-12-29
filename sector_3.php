<?php
ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);

include_once('config.php');

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$AVG_value = isset($_GET['AVG_value']) ? floatval($_GET['AVG_value']) : NULL;

if ($AVG_value === NULL) {
    echo "Error: Missing parameter.";
    exit;  
}

$state = ($AVG_value < 3.0) ? '에러' : '정상'; 

$sql = "UPDATE sector_3 SET AVG_value = ?, state = ?, Times = NOW() WHERE id = 1"; 
$stmt = $conn->prepare($sql);

if ($stmt === false) {
    die('SQL prepare failed: ' . $conn->error);
}

$stmt->bind_param("ds", $AVG_value, $state);

if ($stmt->execute()) {
    if ($stmt->affected_rows > 0) {
        echo "데이터 업데이트 완료";
    } else {
        echo "업데이트된 데이터가 없습니다.";
    }
} else {
    echo "데이터 업데이트 실패: " . $conn->error;
}

$stmt->close();
$conn->close();
?>
