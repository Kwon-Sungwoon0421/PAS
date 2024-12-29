<?php
ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);

include_once('config.php');

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// sector_1~5까지 바꿔주면서 모두 삽입입
$sql = "INSERT INTO sector_5 (id, Times, AVG_value, state) VALUES (?, NOW(), ?, ?)";

$stmt = $conn->prepare($sql);

$id = 1; // id에 1 넣기
$AVG_value = 5.0; // AVG_value에 5.0 넣기
$state = '정상'; // state에 '정상' 넣기

$stmt->bind_param("dss", $id, $AVG_value, $state); 

if ($stmt->execute()) {
    echo "새로운 데이터가 삽입되었습니다.";
} else {
    echo "데이터 삽입 실패: " . $conn->error;
}

$stmt->close();
$conn->close();
?>
