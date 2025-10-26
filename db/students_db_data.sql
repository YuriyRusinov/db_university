-- Insert Students
INSERT INTO Students (first_name, last_name, email, date_of_birth, enrollment_date) VALUES
('John', 'Doe', 'john.doe@university.edu', '2002-05-15', '2020-09-01'),
('Jane', 'Smith', 'jane.smith@university.edu', '2001-08-22', '2020-09-01'),
('Mike', 'Johnson', 'mike.j@university.edu', '2003-01-10', '2021-09-01'),
('Emily', 'Brown', 'emily.b@university.edu', '2002-11-30', '2020-09-01'),
('David', 'Wilson', 'david.w@university.edu', '2001-07-18', '2021-09-01');

-- Insert Student Profiles (1-to-1)
INSERT INTO StudentProfiles (student_id, address, phone_number, emergency_contact, blood_type) VALUES
(1, '123 Main St, City', '555-0101', 'Parent: 555-0102', 'O+'),
(2, '456 Oak Ave, Town', '555-0201', 'Parent: 555-0202', 'A+'),
(3, '789 Pine Rd, Village', '555-0301', 'Parent: 555-0302', 'B+'),
(4, '321 Elm St, City', '555-0401', 'Parent: 555-0402', 'AB+'),
(5, '654 Maple Dr, Town', '555-0501', 'Parent: 555-0502', 'O-');

-- Insert Courses
INSERT INTO Courses (course_code, course_name, credits, department) VALUES
('CS101', 'Introduction to Programming', 3, 'Computer Science'),
('MATH201', 'Calculus I', 4, 'Mathematics'),
('ENG101', 'English Composition', 3, 'English'),
('CS202', 'Data Structures', 4, 'Computer Science'),
('PHYS101', 'Physics I', 4, 'Physics');

-- Insert Enrollments (Many-to-Many)
INSERT INTO Enrollments (student_id, course_id, enrollment_date, grade, semester) VALUES
(1, 1, '2020-09-01', 'A', 'Fall 2020'),
(1, 2, '2020-09-01', 'B+', 'Fall 2020'),
(1, 3, '2020-09-01', 'A-', 'Fall 2020'),
(2, 1, '2020-09-01', 'A-', 'Fall 2020'),
(2, 3, '2020-09-01', 'A', 'Fall 2020'),
(2, 5, '2020-09-01', 'B', 'Fall 2020'),
(3, 1, '2021-09-01', 'B', 'Fall 2021'),
(3, 2, '2021-09-01', 'B+', 'Fall 2021'),
(3, 4, '2021-09-01', 'A', 'Fall 2021'),
(4, 1, '2020-09-01', 'A', 'Fall 2020'),
(4, 2, '2020-09-01', 'A', 'Fall 2020'),
(5, 3, '2021-09-01', 'B+', 'Fall 2021'),
(5, 5, '2021-09-01', 'A-', 'Fall 2021');

-- Insert Assignments (1-to-Many)
INSERT INTO Assignments (course_id, assignment_name, description, due_date, max_points) VALUES
(1, 'Hello World Program', 'Create your first program', '2020-09-15', 10),
(1, 'Variables and Data Types', 'Practice with variables', '2020-09-22', 20),
(1, 'Control Structures', 'If statements and loops', '2020-10-01', 30),
(1, 'Final Project', 'Complete programming project', '2020-12-15', 100),
(2, 'Limits Problem Set', 'Solve limit problems', '2020-09-20', 50),
(2, 'Derivatives Quiz', 'Derivatives assessment', '2020-10-15', 100),
(2, 'Integration Problems', 'Practice integration', '2020-11-10', 75),
(3, 'Essay 1', 'Personal narrative essay', '2020-09-25', 100),
(3, 'Research Paper', 'Academic research paper', '2020-11-20', 200),
(4, 'Arrays and Lists', 'Implement data structures', '2021-09-30', 50),
(5, 'Lab Report 1', 'Mechanics experiment', '2020-10-05', 50);
