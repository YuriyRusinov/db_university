-- =============================================
-- Sample Data
-- =============================================

-- Insert Departments
INSERT INTO Departments (department_code, department_name, chair_person, office_location, budget, established_date) VALUES 
('CS', 'Computer Science', 'Dr. Alice Johnson', 'Tech Building 101', 500000.00, '1990-01-15'),
('MATH', 'Mathematics', 'Dr. Robert Smith', 'Science Hall 201', 350000.00, '1985-08-20'),
('PHYS', 'Physics', 'Dr. Maria Garcia', 'Science Hall 305', 420000.00, '1988-03-10'),
('ENG', 'English Literature', 'Dr. James Wilson', 'Humanities Building 102', 280000.00, '1975-09-01');

-- Insert Students
INSERT INTO Students (student_number, first_name, middle_name, last_name, email, date_of_birth, enrollment_date) VALUES 
('S2023001', 'John', 'Michael', 'Doe', 'john.doe@university.edu', '2000-05-15', '2023-09-01'),
('S2023002', 'Jane', NULL, 'Smith', 'jane.smith@university.edu', '1999-08-22', '2023-09-01'),
('S2023003', 'Robert', 'James', 'Johnson', 'robert.johnson@university.edu', '2001-03-10', '2023-09-01'),
('S2023004', 'Maria', 'Isabel', 'Garcia', 'maria.garcia@university.edu', '2000-11-30', '2023-09-01'),
('S2023005', 'David', 'William', 'Brown', 'david.brown@university.edu', '2002-02-14', '2023-09-01');

-- Insert Student Profiles
INSERT INTO Student_Profiles (student_id, phone_number, emergency_contact_name, emergency_contact_phone, address, city, state, postal_code) VALUES 
(1, '+1-555-0101', 'Michael Doe', '+1-555-0102', '123 Main Street', 'Boston', 'MA', '02101'),
(2, '+1-555-0103', 'John Smith', '+1-555-0104', '456 Oak Avenue', 'Cambridge', 'MA', '02138'),
(3, '+1-555-0105', 'Sarah Johnson', '+1-555-0106', '789 Pine Road', 'Boston', 'MA', '02101'),
(4, '+1-555-0107', 'Carlos Garcia', '+1-555-0108', '321 Elm Street', 'Boston', 'MA', '02101'),
(5, '+1-555-0109', 'Lisa Brown', '+1-555-0110', '654 Maple Drive', 'Cambridge', 'MA', '02138');

-- Insert Courses
INSERT INTO Courses (course_code, course_name, description, credits, department_id, instructor_name, max_capacity) VALUES 
('CS101', 'Introduction to Programming', 'Fundamental programming concepts and problem-solving techniques', 3, 1, 'Dr. Alice Johnson', 30),
('CS201', 'Data Structures', 'Advanced data structures and algorithms analysis', 4, 1, 'Dr. Brian Davis', 25),
('MATH101', 'Calculus I', 'Differential calculus and applications', 4, 2, 'Dr. Robert Smith', 35),
('MATH202', 'Linear Algebra', 'Vector spaces and linear transformations', 3, 2, 'Dr. Sarah Chen', 28),
('PHYS101', 'General Physics I', 'Mechanics, heat, and sound', 4, 3, 'Dr. Maria Garcia', 30),
('ENG201', 'British Literature', 'Survey of British literature from Chaucer to present', 3, 4, 'Dr. James Wilson', 25);

-- Insert Enrollments
INSERT INTO Enrollments (student_id, course_id, enrollment_date, grade, status, semester, credit_hours) VALUES 
(1, 1, '2023-09-01', 85.5, 'completed', 'Fall2023', 3),
(1, 3, '2023-09-01', 92.0, 'completed', 'Fall2023', 4),
(1, 2, '2024-01-15', NULL, 'active', 'Spring2024', 4),
(2, 1, '2023-09-01', 78.0, 'completed', 'Fall2023', 3),
(2, 4, '2024-01-15', NULL, 'active', 'Spring2024', 3),
(3, 1, '2023-09-01', 91.5, 'completed', 'Fall2023', 3),
(3, 5, '2023-09-01', 88.0, 'completed', 'Fall2023', 4),
(4, 6, '2023-09-01', 95.0, 'completed', 'Fall2023', 3),
(4, 2, '2024-01-15', NULL, 'active', 'Spring2024', 4),
(5, 3, '2023-09-01', 82.5, 'completed', 'Fall2023', 4),
(5, 1, '2024-01-15', NULL, 'active', 'Spring2024', 3);

