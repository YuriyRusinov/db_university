-- 1. Get all students with their profiles (1-to-1)
SELECT 
    s.student_id,
    s.first_name,
    s.last_name,
    s.email,
    sp.phone_number,
    sp.address,
    sp.blood_type
FROM Students s
LEFT JOIN StudentProfiles sp ON s.student_id = sp.student_id;

-- 2. Get all courses with their assignments count (1-to-Many)
SELECT 
    c.course_code,
    c.course_name,
    c.department,
    COUNT(a.assignment_id) as assignment_count
FROM Courses c
LEFT JOIN Assignments a ON c.course_id = a.course_id
GROUP BY c.course_id, c.course_code, c.course_name, c.department
ORDER BY assignment_count DESC;

-- 3. Get all students enrolled in a specific course (Many-to-Many)
SELECT 
    c.course_code,
    c.course_name,
    s.first_name,
    s.last_name,
    e.grade,
    e.semester
FROM Enrollments e
JOIN Students s ON e.student_id = s.student_id
JOIN Courses c ON e.course_id = c.course_id
WHERE c.course_code = 'CS101'
ORDER BY s.last_name;

-- 4. Get student enrollment summary
SELECT 
    s.first_name || ' ' || s.last_name as student_name,
    COUNT(e.enrollment_id) as courses_enrolled,
    ROUND(AVG(
        CASE e.grade
            WHEN 'A' THEN 4.0
            WHEN 'A-' THEN 3.7
            WHEN 'B+' THEN 3.3
            WHEN 'B' THEN 3.0
            WHEN 'B-' THEN 2.7
            ELSE 2.0
        END
    ), 2) as gpa
FROM Students s
LEFT JOIN Enrollments e ON s.student_id = e.student_id
GROUP BY s.student_id, student_name
ORDER BY gpa DESC NULLS LAST;

-- 5. Get courses with assignments due soon
SELECT 
    c.course_name,
    a.assignment_name,
    a.due_date,
    a.max_points,
    COUNT(e.student_id) as enrolled_students
FROM Assignments a
JOIN Courses c ON a.course_id = c.course_id
LEFT JOIN Enrollments e ON c.course_id = e.course_id
WHERE a.due_date >= CURRENT_DATE
GROUP BY c.course_name, a.assignment_name, a.due_date, a.max_points
ORDER BY a.due_date;
