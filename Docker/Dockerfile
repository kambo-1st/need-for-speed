FROM php:8.2-cli-bookworm

RUN apt-get update && \
    apt-get -y install \
            build-essential \
            autoconf \
            automake \
            gosu \
            sudo \
            libffi-dev \
            libclang-dev \
            unzip && \
    	docker-php-ext-configure ffi && \
    	docker-php-ext-install -j$(nproc) ffi && \
        docker-php-ext-install opcache && \
        apt-get clean && \
        docker-php-source delete && \
        rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

RUN curl -sS https://getcomposer.org/installer | php -- --install-dir=/usr/local/bin --filename=composer
#RUN curl https://sh.rustup.rs -sSf | sh -s -- -y

# Add local user 'dev'
RUN groupadd -r dev --gid=9001 && useradd -r -g dev --uid=9001 dev
# Grant him sudo privileges
RUN echo "dev ALL=(root) NOPASSWD:ALL" > /etc/sudoers.d/dev && \
    chmod 0440 /etc/sudoers.d/dev

# Do stuff with this user if needed
USER dev
ENV HOME /home/dev
WORKDIR $HOME

#RUN curl https://sh.rustup.rs -sSf | sh -s -- -y

# Repass root
USER root

RUN echo 'alias ll="ls -all"' >> /home/dev/.bashrc

RUN curl https://sh.rustup.rs -sSf | sh -s -- -y

# set user rights for dev in folder /home/dev/.cargo/
RUN chown -R dev:dev /home/dev/.cargo/

# Copy entrypoint
COPY Docker/docker-entrypoint.sh /usr/local/bin/docker-entrypoint.sh
RUN ["chmod", "+x", "/usr/local/bin/docker-entrypoint.sh"]
ENTRYPOINT ["/usr/local/bin/docker-entrypoint.sh"]

COPY Docker/opcache.ini /usr/local/etc/php/conf.d/opcache.ini

CMD ["bash"]
