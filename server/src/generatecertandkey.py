from cryptography import x509
from cryptography.x509.oid import NameOID
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.asymmetric import rsa
from cryptography.hazmat.primitives import serialization
from cryptography.hazmat.primitives.serialization import NoEncryption
from datetime import datetime, timedelta
import ipaddress

# Generate a private key
private_key = rsa.generate_private_key(
    public_exponent=65537,
    key_size=2048,
)

# Self-signed certificate details
subject = issuer = x509.Name([
    x509.NameAttribute(NameOID.COUNTRY_NAME, "US"),
    x509.NameAttribute(NameOID.STATE_OR_PROVINCE_NAME, "California"),
    x509.NameAttribute(NameOID.LOCALITY_NAME, "Mountain View"),
    x509.NameAttribute(NameOID.ORGANIZATION_NAME, "My Organization"),
    x509.NameAttribute(NameOID.COMMON_NAME, "myserver.example.com"),
])

# Validity period
valid_from = datetime.utcnow()
valid_to = valid_from + timedelta(days=365)

# Create a self-signed certificate
certificate = x509.CertificateBuilder().subject_name(
    subject
).issuer_name(
    issuer
).public_key(
    private_key.public_key()
).serial_number(
    x509.random_serial_number()
).not_valid_before(
    valid_from
).not_valid_after(
    valid_to
).add_extension(
    x509.SubjectAlternativeName([x509.DNSName("myserver.example.com")]),
    critical=False,
).sign(private_key, hashes.SHA256())

# Serialize private key and certificate
private_key_pem = private_key.private_bytes(
    encoding=serialization.Encoding.PEM,
    format=serialization.PrivateFormat.TraditionalOpenSSL,
    encryption_algorithm=NoEncryption()
)

certificate_pem = certificate.public_bytes(serialization.Encoding.PEM)

# Save the private key and certificate to files
private_key_path = "serverKey.pem"
certificate_path = "serverCert.pem"

# Writing the private key
with open(private_key_path, "wb") as private_key_file:
    private_key_file.write(private_key_pem)

# Writing the certificate
with open(certificate_path, "wb") as certificate_file:
    certificate_file.write(certificate_pem)