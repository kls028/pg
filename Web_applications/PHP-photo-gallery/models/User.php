<?php
class User {
    private $db;

    public function __construct($db) {
        $this->db = $db;
    }

    public function register($username, $email, $password) {
        $hashedPassword = password_hash($password, PASSWORD_BCRYPT);
        $this->db->users->insertOne([
            'username' => $username,
            'email' => $email,
            'password' => $hashedPassword
        ]);
    }

    public function login($username, $password) {
        $user = $this->db->users->findOne(['username' => $username]);
        if ($user && password_verify($password, $user->password)) {
            return $user;
        }
        return null;
    }
    
    public function getUsername($userId) {
        $user = $this->db->users->findOne(['_id' => new MongoDB\BSON\ObjectId($userId)]);
        return $user ? $user['username'] : null;
    }
    public function isLoggedIn() {
        return isset($_SESSION['user_id']);
    }
    public function findByUsername($username) {
        $collection = $this->db->users;
        return $collection->findOne(['username' => $username]);
    }
    public function findByEmail($email) {
        $collection = $this->db->users;
        return $collection->findOne(['email' => $email]);
    }
}
?>
