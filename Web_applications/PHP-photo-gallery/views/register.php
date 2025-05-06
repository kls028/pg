<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Register - Sudoku Gallery</title>
    <link rel="stylesheet" href="styles/login-style.css">
</head>
<body>
    <div class="login-container">
        <?php if (!empty($error)) : ?>
            <div class="error-message"><?php echo htmlspecialchars($error); ?></div>
        <?php endif; ?>
        
        <h2>Register</h2>
        <form method="POST" action="app.php?controller=user&action=registerAction" class="login-form">
            <label for="username">Username:</label>
            <input type="text" name="username" id="username" required><br><br>

            <label for="email">Email:</label>
            <input type="email" name="email" id="email" required><br><br>

            <label for="password">Password:</label>
            <input type="password" name="password" id="password" required><br><br>
            
            <label for="password_confirm">Repeat Password:</label>
            <input type="password" name="password_confirm" id="password_confirm" required><br><br>

            <button type="submit" class="login-btn">Register</button>
        </form>
        <div class="login-footer">
            <p>Already have an account? <a href="app.php?controller=user&action=loginAction">Login here</a></p>
        </div>
    </div>
</body>
</html>
