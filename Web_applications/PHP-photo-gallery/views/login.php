<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Login - Sudoku Gallery</title>
    <link rel="stylesheet" href="styles/login-style.css">
    <script>
        function redirectAfterDelay() {
            setTimeout(function() {}
            , 2000); // 2000ms = 2 seconds
        }
    </script>
</head>
<body>
    <div class="login-container">
        <?php if (!empty($error)) : ?>
            <div class="error-message"><?php echo htmlspecialchars($error); ?></div>
        <?php endif; ?>
        <?php if(!empty($success)): ?>
            <div class="success-message"><?php echo htmlspecialchars($success); ?></div>
            <script>redirectAfterDelay();</script>
        <?php endif; ?>
        <h2>Login</h2>
        <form method="POST" action="app.php?controller=user&action=loginAction" class="login-form">
            <label for="username">Username:</label>
            <input type="username" name="username" id="username" required><br><br>

            <label for="password">Password:</label>
            <input type="password" name="password" id="password" required><br><br>

            <button type="submit" class="login-btn">Login</button>
        </form>
        <div class="login-footer">
            <p>Don't have an account? <a href="app.php?controller=user&action=registerAction">Register here</a></p>
            <p><a href="app.php?controller=gallery&action=galleryAction">Want to visit as a guest?</a></p>
        </div>
    </div>
</body>
</html>
